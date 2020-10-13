#include <memory>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/Linker/Linker.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IR/Verifier.h>
#include "ipc_module.h"
#include "llvm_module.h"
#include "../stdlib-is-shit.h"
#include "../cali_linker/archive-wrapper.h"
#include "../cali_linker/debug.h"
#include "../cali_linker/linker_replacement.h"
#include "../cali_linker/randomness.h"


namespace ipcrewriter {


	std::shared_ptr<IpcModule>
	IpcModule::newIpcModuleFromFile(const std::string &filename, bool isMainModule, YamlConfig *config, const ContextConfig *contextConfig) {
		if (endsWith(filename, ".bc") || endsWith(filename, ".ll") || endsWith(filename, ".o")) {
			return std::shared_ptr<IpcModule>(new LlvmIpcModule(filename, isMainModule, config, contextConfig));
		}
		if (endsWith(filename, ".so") || endsWith(filename, ".a")) {
			return std::shared_ptr<IpcModule>(new BinaryIpcModule(filename, isMainModule, config, contextConfig));
		}
		throw std::invalid_argument("No supported extension");
	}

	const std::set<std::string> &IpcModule::getImports() const {
		return imports;
	}

	const std::set<std::string> &IpcModule::getExports() const {
		return exports;
	}

	const std::string &IpcModule::getSource() const {
		return source;
	}

	const std::vector<std::string> &IpcModule::getLogEntries() {
		return logEntries;
	}




	static int linked_things = 0;

	std::shared_ptr<IpcModule>
	CompositeIpcModule::newIpcModulesFromFiles(std::vector<std::string> &files, bool isMainModule, YamlConfig *config, const ContextConfig *contextConfig,
											   const std::string &output_filename) {
		std::vector<std::shared_ptr<IpcModule>> binary_modules;
		std::set<std::string> seen_files;
		std::set<std::string> ignored;

		// Prepare LLVM linker
		LlvmIpcModule::context.enableDebugTypeODRUniquing();
		// Load initial llvm bitcode file, containing some libc stubs
		filesystem::path stubsFilename = applicationPath;
		stubsFilename.append("libc-stubs.bc");
		llvm::SMDiagnostic error;
		auto composite_module = parseIRFile(stubsFilename.string(), error, LlvmIpcModule::context);
		composite_module->setModuleIdentifier("llvm-linked-things_" + output_filename + "_" + std::to_string(linked_things++) + '-' + getRandomString() + ".bc");

		// Prepare linker
		llvm::Linker L(*composite_module);
		int linked_modules = 0;
		auto linkerflags = config->linkerOverride ? llvm::Linker::Flags::OverrideFromSrc : llvm::Linker::Flags::None;

		// Helper function - link an additional LLVM file to the unique LLVM module
		auto addLlvmModule = [&linked_modules, &L, linkerflags](std::unique_ptr<llvm::Module> module) {
			if (L.linkInModule(std::move(module), linkerflags))
				throw std::runtime_error("Could not link module");
			linked_modules++;
		};

		for (const auto &filename: files) {
			// file missing?
			if (!exists(filename)) {
				std::cerr << "Warning: file not found (" << filename << ")" << std::endl;
				continue;
			}
			// Check if file has already been loaded
			if (!seen_files.insert(absolute(filename)).second) {
				std::cerr << "Already seen: " << filename << std::endl;
				continue;
			}

			// Shared libraries
			if (endsWith(filename, ".so")) {
				binary_modules.push_back(std::shared_ptr<IpcModule>(new BinaryIpcModule(filename, isMainModule, config, contextConfig)));
				continue;
			}

			// LLVM files
			if (endsWith(filename, ".bc") || endsWith(filename, ".ll")) {
				auto m = llvm::parseIRFile(filename, error, LlvmIpcModule::context);
				if (filename.find("libstdc++") != std::string::npos)
					for (auto &g: m->functions())
						if (g.hasName()) ignored.insert(g.getName());
				addLlvmModule(std::move(m));
			}

			// object files
			if (endsWith(filename, ".o")) {
				auto header = read_file_limit(filename, 4);
				if (header == "BC\xc0\xde") {
					addLlvmModule(llvm::parseIRFile(filename, error, LlvmIpcModule::context));
				} else if (header == "\x7f""ELF") {
					binary_modules.push_back(std::shared_ptr<IpcModule>(new BinaryIpcModule(filename, isMainModule, config, contextConfig)));
				} else {
					std::cerr << "Can\'t determine type of file " + filename << std::endl;
				}
			}

			// static libraries
			if (endsWith(filename, ".a")) {
				bool binary_added = false;
				dbg_cout << "Open archive " << filename << std::endl;
				libarchive::Archive archive(filename);
				int i = 0;
				for (auto it: archive) {
					//if (it.name() != "magick_libMagickCore_6_Q16_la-ps.o" && it.name() != "magick_libMagickCore_6_Q16_la-string.o")
					//	continue; //TODO hack
					// if (i++ == 3)
					//	break; //TODO hack
					if (endsWith(it.name(), ".o") || endsWith(it.name(), ".bc") || endsWith(it.name(), ".lo")) {
						dbg_cout << "- Archive entry: " << it.name() << std::endl;
						std::string buffer = it.read();
						if (buffer.substr(0, 4) == "BC\xc0\xde") {
							auto buffer2 = llvm::MemoryBuffer::getMemBufferCopy(buffer);
							auto m = llvm::parseIR(buffer2->getMemBufferRef(), error, LlvmIpcModule::context);
							if (!m)
								error.print(it.name().c_str(), llvm::errs());
							if (filename.find("libstdc++") != std::string::npos)
								for (auto &g: m->functions())
									if (g.hasName()) ignored.insert(g.getName());
							addLlvmModule(std::move(m));
						} else if (buffer.substr(0, 4) == "\x7f""ELF") {
							if (!binary_added) {
								binary_modules.push_back(std::shared_ptr<IpcModule>(new BinaryIpcModule(filename, isMainModule, config, contextConfig, true)));
								binary_added = true;
							}
						} else {
							std::cerr << "Can\'t determine type of file " + it.name() << " in archive " << filename << std::endl;
						}
					}
				}
			}
		}

		if (llvm::verifyModule(*composite_module, &llvm::errs())) {
			throw std::runtime_error("linked module is broken!");
		}

		// build composite
		if (linked_modules == 0 && binary_modules.size() == 1)
			return binary_modules[0];
		if (linked_modules == 1 && binary_modules.empty())
			return std::shared_ptr<IpcModule>(new LlvmIpcModule(std::move(composite_module), isMainModule, config, contextConfig));

		auto m = std::make_shared<CompositeIpcModule>(isMainModule, config, contextConfig);
		if (linked_modules > 0) {
			auto llvmModule = std::shared_ptr<IpcModule>(new LlvmIpcModule(std::move(composite_module), isMainModule, config, contextConfig));
			dbg_cout << ignored.size() << " ignored symbols" << std::endl;
			llvmModule->ignored = std::move(ignored);
			m->add(llvmModule);
		}
		for (const auto &bm: binary_modules)
			m->add(bm);
		return m;
	}

	CompositeIpcModule::CompositeIpcModule(bool isMainModule, YamlConfig *config, const ContextConfig *contextConfig)
			: IpcModule("", isMainModule, config, contextConfig) {}


	const std::vector<std::string> &CompositeIpcModule::getLogEntries() {
		logEntries.clear();
		for (auto &m: modules) {
			for (auto &s: m->getLogEntries())
				logEntries.push_back(s);
		}
		return logEntries;
	}
}