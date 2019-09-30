
#include "llvm_module.h"
#include "../passes/PDGReachabilityPass.h"
#include "../passes/PDGCreationPass.h"
#include "../passes/PDGWriterPass.h"
#include "../passes/PDGSharedMemoryPass.h"
#include "../passes/PDGInstrumentPass.h"
#include "../passes/PDGSpecializationPass.h"
#include "../passes/DataDrivenSCCPass.h"
#include "../passes/FunctionAliasResolverPass.h"
#include "../cali_linker/debug.h"
#include <iostream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Analysis/MemoryDependenceAnalysis.h>
#include <llvm/IR/Verifier.h>
#include <llvm/CodeGen/DIE.h>
#include <llvm/CodeGen/UnreachableBlockElim.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/LinkAllPasses.h>
#include <fstream>

namespace ipcrewriter {

	using namespace std;
	using namespace llvm;
	using namespace llvm::legacy;

	llvm::LLVMContext LlvmIpcModule::context;

	LlvmIpcModule::LlvmIpcModule(const std::string &source, bool mainModule, YamlConfig *config, const ContextConfig *contextConfig)
			: IpcModule(source, mainModule, config, contextConfig) {}

	void LlvmIpcModule::loadModuleFile() {
		if (!module) {
			SMDiagnostic error;
			module = std::move(parseIRFile(source, error, context));
			if (!module) throw runtime_error("Could not read/parse IR file");
		}

		// find imports / exports
		for (const auto &function : module->functions()) {
			if (function.isDeclaration()) {
				imports.insert(function.getGlobalIdentifier());
			} else {
				exports.insert(function.getGlobalIdentifier());
			}
		}
	}

	void LlvmIpcModule::rewriteFunctionCalls(CommunicationPair &cmp, const std::set<std::string> &symbols) {
		runAnalysisTodo(symbols);

		for (auto &function : module->functions()) {
			if (!function.isDeclaration()) continue;
			StringRef name = function.getName();
			if (symbols.find(name) != symbols.end()) {
				// Function *replacement = cmp.getOrCreateWrapperFunction(function, this, &callInst);
				while (function.use_begin() != function.use_end()) {
					auto &use = *function.use_begin();
					if (isa<CallInst>(use.getUser()) && cast<CallInst>(use.getUser())->getCalledFunction() == &function) {
						dbg_llvm_outs << "Replacing in call " << *use.getUser() << "\n";
						Function *replacement = cmp.getOrCreateWrapperFunction(&function, this, cast<CallInst>(use.getUser()));
						dbg_llvm_outs << "   " << *cast<CallInst>(use.getUser())->getCalledFunction()->getType() << "   =>   " << *replacement->getType() << "\n";
						// use.set(replacement);
						cast<CallInst>(use.getUser())->setCalledFunction(replacement);
					} else if (isa<InvokeInst>(use.getUser()) && cast<InvokeInst>(use.getUser())->getCalledFunction() == &function) {
						dbg_llvm_outs << "Replacing in invoke " << *use.getUser() << "\n";
						Function *replacement = cmp.getOrCreateWrapperFunction(&function, this, cast<InvokeInst>(use.getUser()));
						dbg_llvm_outs << "   " << *cast<InvokeInst>(use.getUser())->getCalledFunction()->getType() << "   =>   " << *replacement->getType() << "\n";
						// use.set(replacement);
						cast<InvokeInst>(use.getUser())->setCalledFunction(replacement);
					} else {
						dbg_llvm_outs << "Replacing in ???? " << *use.getUser() << "\n";
						Function *replacement = cmp.getOrCreateWrapperFunction<CallInst>(&function, this, nullptr);
						use.set(replacement);
						dbg_llvm_outs << "              =>  " << *use.getUser() << "\n";
						break;
					}
				}
			}
			/*
			for (auto &bb : function) {
				for (auto ins = bb.begin(), insend = bb.end(); ins != insend; ins++) {
					if (isa<CallInst>(ins)) {
						auto &callInst = cast<CallInst>(*ins);
						auto calledFunction = callInst.getCalledFunction();
						if (calledFunction) {
							StringRef name = calledFunction->getName();
							if (symbols.find(name) != symbols.end()) {
								// this is a call to the other module!
								Function *replacement = cmp.getOrCreateWrapperFunction(calledFunction, this, &callInst);
								// errs() << "replacing " << name << " with " << replacement->getName() << " in " << *ins << "\n";
								callInst.setCalledFunction(replacement);
							}
						}
					}
				}
			}
			*/
		}
	}

	void LlvmIpcModule::save() {
		error_code ec;
		raw_fd_ostream os(rewrittenFilename()[0], ec, llvm::sys::fs::F_None);
		WriteBitcodeToFile(*module, os);
		assert(!ec);
	}

	std::vector<std::string> LlvmIpcModule::rewrittenFilename() {
		return {source.substr(0, source.length() - 3) + "-ipc.bc"};
	}

	void LlvmIpcModule::runAnalysisTodo(const set<string> &symbols) {
		//TODO
		usleep(10000);
		auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
		cout << "--- Passes running now ---" << endl;
		// outs() << *module << "\n";

		legacy::PassManager pm;
		pm.add(createVerifierPass(true));
		pm.add(createPromoteMemoryToRegisterPass());
		pm.add(createAggressiveDCEPass());
		pm.add(createGlobalDCEPass());
		pm.add(createCFGSimplificationPass());
		pm.add(createUnifyFunctionExitNodesPass());

		// pm.add(new MemoryDependenceWrapperPass());

		pm.add(new FunctionAliasResolverPass());
		pm.add(new DataDrivenSCCPass());
		pm.add(new PDGCreationPass(config, symbols));
		pm.add(new PDGReachabilityPass(config));
		pm.add(new PDGSpecializationPass(config));
		if (config && config->writeGraphs) {
			auto pass = new PDGWriterPass();
			pass->runDot = config->writeGraphsDot;
			pm.add(pass);
		}
		if (contextConfig && contextConfig->instrument) {
			cout << "Instrumentation scheduled..." << endl;
			pm.add(new PDGInstrumentPass(ignored));
		}
		pm.add(new PDGSharedMemoryPass(config, symbols));
		pm.add(createVerifierPass(true));
		// pm.add(new CallGraphWrapperPass());
		// pm.add(new OldSCCAnalysis());
		pm.run(*module);

		llvm::outs().flush();
		auto dt = std::chrono::high_resolution_clock::now().time_since_epoch() - t;
		usleep(10000);
		cout << "\n--- Passes finished (in " << std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() << " ms) ---" << endl;
	}

}