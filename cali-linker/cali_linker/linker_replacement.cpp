#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <boost/optional.hpp>
#include "linker_replacement.h"
#include "ipc_rewriter.h"
#include "../modules/ipc_module.h"
#include "../stdlib-is-shit.h"
#include "debug.h"


filesystem::path applicationPath;


using namespace ipcrewriter;
using namespace std;


class LibraryContext {
	std::shared_ptr<IpcModule> module = nullptr;
public:
	YamlConfig *globalConfig;
	const ContextConfig *config;

	bool is_main_module = false;
	// old files
	vector<string> files;
	// new files
	bool is_rewritten = false;
	vector<string> rewritten_args;

	LibraryContext(YamlConfig *globalConfig, const ContextConfig *config) : globalConfig(globalConfig), config(config) {}


	std::shared_ptr<IpcModule> get_module(const std::string& output_filename) {
		// TODO
		if (!module) {
			// module = IpcModule::newIpcModuleFromFile(get_single_file(), is_main_module, false);
			module = CompositeIpcModule::newIpcModulesFromFiles(files, is_main_module, globalConfig, config, output_filename);
		}
		return module;
	}

	void set_output_files() {
		if (module && module->canRewrite()) {
			for (const auto &fname: module->rewrittenFilename())
				rewritten_args.push_back(fname);
			is_rewritten = true;
		}
	}

	void print() {
#ifndef SILENT
		cout << "Library Context \"" << config->name << "\": " << endl;
		for (const auto &f: files)
			cout << "  - " << f << endl;
		if (files.empty())
			cout << "  (no files)" << endl;
#endif
	}
};




class LinkerArguments {
	vector<string> arguments{};
	vector<string> librarySearchpath{"."};
	// vector<string> objects;
	// vector<string> libraries;
	map<int, std::shared_ptr<LibraryContext>> argumentUsage;
	vector<std::shared_ptr<LibraryContext>> libraries;
	YamlConfig config;
	std::string output_filename;
	bool is_shared_library = false;

public:

	LinkerArguments(int argc, const char *argv[]) {
		dbg_cerr << "\n\nLinker Arguments:" << std::endl;
		for (int i = 1; i < argc; i++) {
			dbg_cerr << "   " << argv[i] << std::endl;
			std::string arg = argv[i];
			//TODO hacky workaround
			arg = replaceAll(arg, "/usr/lib/llvm-5.0", "/usr/lib/llvm-7");
			arguments.push_back(arg);
		}
		dbg_cerr << "\n\n" << std::endl;

		// Find config file
		string configfile = "config.yaml";
		for (const auto &s: arguments) {
			if (startsWith(s, "--cali-config=")) {
				configfile = s.substr(14);
			} else if (startsWith(s, "--ipc-config=")) {
				configfile = s.substr(13);
			}
		}
		if (configfile[0] != '/' && !filesystem::exists(configfile)) {
			configfile = applicationPath.parent_path().parent_path().append("cali-linker").append("sample_configs").append(configfile).string();
		}
		config = YamlConfig::fromFile(configfile);
		modifyArguments();

		// Parse other commandline arguments
		bool skipNext = false;
		bool nextOutput = false;
		for (const auto &s: arguments) {
			if (skipNext) {
				skipNext = false;
			} else if (nextOutput) {
				output_filename = s;
				nextOutput = false;
			} else if (startsWith(s, "-L")) {
				librarySearchpath.push_back(s.substr(2));
			} else if (s == "-o") {
				nextOutput = true;
			} else if (s == "-plugin" || s == "-rpath") {
				skipNext = true;
			} else if (s == "-shared") {
				is_shared_library = true;
			}
		}
	}

	void modifyArguments() {
		for (ssize_t i = arguments.size() - 1; i >= 0; i--) {
			if (startsWith(arguments[i], "--ipc-config=") || startsWith(arguments[i], "--cali-config=")) {
				arguments.erase(arguments.begin() + i);
			}
			auto it = config.replaceArguments.find(arguments[i]);
			if (it != config.replaceArguments.end()) {
				if (it->second.empty()) {
					arguments.erase(arguments.begin() + i);
				} else {
					arguments[i] = it->second[0];
					for (size_t j = 1; j < it->second.size(); j++) {
						arguments.insert(arguments.begin()+(i+j), it->second[j]);
					}
				}
			}
		}
	}

	void build_libraries() {
		libraries.clear();
		map<string, std::shared_ptr<LibraryContext>> known_libs;

		auto add_file_to_library_list = [this, &known_libs](const string &filename, int i) {
			for (const auto &item: config.contexts) {
				if (item.second.matches(filename)) {
					dbg_cout << "match: " << filename << " " << item.second.name << endl;
					auto it = known_libs.find(item.second.name);
					auto library_context =
							it != known_libs.end()
							// existing library context
							? it->second
							// new library context
							: (libraries.push_back(known_libs[item.second.name] = make_shared<LibraryContext>(&config, &item.second)),
									libraries[libraries.size() - 1]);
					if (std::find(library_context->files.begin(), library_context->files.end(), filename) == library_context->files.end())
						library_context->files.push_back(filename);
					if (i >= 0)
						argumentUsage[i] = library_context;
					break;
				}
			}
		};

		for (size_t i = 0; i < arguments.size(); i++) {
			const auto &s = arguments[i];
			if (endsWith(s, ".la")) {
				if (replace_libtool_in_arguments(s, i)) {
					i--;
				}
			} else if (config.addLibstdcxx && s == "-lstdc++") {
				add_file_to_library_list(applicationPath.parent_path().append("libstdc++.bc").string(), -1);
				cerr << "Add libstdc++.bc" << endl;
			} else if (startsWith(s, "-l") || endsWith(s, ".so") || endsWith(s, ".a")) {
				auto lib = find_library(startsWith(s, "-l") ? "lib" + s.substr(2) : s);
				if (lib) {
					add_file_to_library_list(*lib, i);
				}
			} else if ((endsWith(s, ".o") || endsWith(s, ".bc") || endsWith(s, ".lo")) && !startsWith(s, "/usr/")) {
				add_file_to_library_list(s, i);
			} else if (s == "-o" || s == "-plugin" || s == "-rpath") {
				i++;
			}
		}

		auto it = known_libs.find("main");
		if (it != known_libs.end()) {
			it->second->is_main_module = true;
		} else if (!libraries.empty()) {
			libraries[0]->is_main_module = true;
		}
	}

	/**
	 *
	 * @param libtoolFile
	 * @param argNumber
	 * @return true if the libtool file has been replaced by other arguments
	 */
	bool replace_libtool_in_arguments(const std::string &libtoolFile, int argNumber) {
		std::ifstream f(libtoolFile);
		std::string line;
		std::string library;
		std::vector<std::string> additionalFlags;

		auto addFlags = [](const std::string &text, std::vector<std::string> &additionalFlags) {
			size_t last = 0;
			size_t next = 0;
			while ((next = text.find(' ', last)) != string::npos) {
				if (next - last > 1) {
					auto s = text.substr(last, next - last);
					if (s != "-pthread")
						additionalFlags.push_back(s);
				}
				last = next + 1;
			}
			if (text.size() - last > 1) {
				auto s = text.substr(last);
				if (s != "-pthread")
					additionalFlags.push_back(s);
			}
		};

		while (std::getline(f, line)) {
			if (startsWith(line, "old_library='")) {
				library = line.substr(13, line.size() - 14);
				auto p = libtoolFile.find_last_of('/');
				if (p != std::string::npos && library[0] != '/') {
					library = libtoolFile.substr(0, p + 1) + ".libs/" + library;
				}
			} else if (startsWith(line, "inherited_linker_flags='")) {
				addFlags(line.substr(24, line.size() - 25), additionalFlags);
			} else if (startsWith(line, "dependency_libs='")) {
				addFlags(line.substr(17, line.size() - 18), additionalFlags);
			}
		}

		arguments[argNumber] = library;
		arguments.insert(arguments.begin() + argNumber + 1, additionalFlags.begin(), additionalFlags.end());
		/*std::cerr << "PATCHED " << libtoolFile << " at " << argNumber << " : \n";
		for (auto &s: arguments) std::cerr << s << " ";
		std::cerr << std::endl;*/
		return true;
	}

	void link() {
		// if this output should not be processed - delegate to the real linker now
		if (std::find(config.ignoreOutputs.begin(), config.ignoreOutputs.end(), output_filename) != config.ignoreOutputs.end()) {
			execute_real_linker();
			return;
		}

		build_libraries();

		dbg_cout << libraries.size() << " library contexts." << endl;
		for (const auto &l: libraries) l->print();

		if (changesRequired()) {
			prepare_ipc();
		}

		execute_real_linker();
	}

	/**
	 * Find the actual file belonging to a library parameter
	 * @param name
	 * @return
	 */
	boost::optional<string> find_library(const string &name) {
		if (exists(name)) return name;
		for (const auto &sp: librarySearchpath) {
			string fname = sp + "/";
			fname += name;
			if (exists(fname)) return fname;
			fname += ".so";
			if (exists(fname)) return fname;
			fname = sp + "/";
			fname += name + ".a";
			if (exists(fname)) return fname;
		}
		cerr << "Warning: Library not found (" << name << ")" << endl;
		return {};
	}

	bool changesRequired() {
		return libraries.size() >= 2 && !is_shared_library;
	}

	void prepare_ipc() {
		static const std::vector<std::string> libipc_args = {
				"--whole-archive",
				applicationPath.parent_path().append("libipc").append("libipc.a").string(),
				applicationPath.parent_path().append("libnsjail").append("libnsjail.a").string(),
				applicationPath.parent_path().append("libnsjail").append("libkafel.a").string(),
				"--no-whole-archive",
				"-ldl",
				"-lrt",
				"-lnl-3",
				"-lnl-route-3",
				"-lpthread",
				"-lstdc++"
		};

		IpcRewriter ipcRewriter;
		const ContextConfig *contextConfig = nullptr;
		auto p = output_filename.rfind('/');
		auto output_name = p == string::npos ? output_filename : output_filename.substr(p+1);
		for (const auto &lib: libraries) {
			ipcRewriter.addModule(lib->get_module(output_name));
			if (!lib->is_main_module)
				contextConfig = lib->config;
		}

		ipcRewriter.createIpcJail(contextConfig, &config);
		ipcRewriter.saveAllModules(filesystem::current_path().string());
		ipcRewriter.saveLogEntries(output_filename + ".log");

		for (const auto &lib: libraries) {
			if (lib->is_main_module) {
				for (const auto &arg: libipc_args) {
					if (lib->config->instrument) {
						lib->rewritten_args.push_back(replaceAll(arg, "libipc.a", "libipc-instrument.a"));
					} else if (lib->config->mprotect_mode) {
						lib->rewritten_args.push_back(replaceAll(arg, "libipc.a", "libipc-mprotect.a"));
					} else if (lib->config->sequential_mode) {
						lib->rewritten_args.push_back(replaceAll(arg, "libipc.a", "libipc-sequential.a"));
					} else {
						lib->rewritten_args.push_back(arg);
					}
				}
				for (const auto &fname: ipcRewriter.new_files)
					lib->rewritten_args.push_back(fname);
			}
			lib->set_output_files();
		}
	}

	vector<string> build_linker_argument_list() {
		vector<string> arg2;
		set<LibraryContext *> processed_libraries;
		for (size_t i = 0; i < arguments.size(); i++) {
			const auto it = argumentUsage.find(i);
			if (it != argumentUsage.end() && it->second->is_rewritten) {
				if (processed_libraries.insert(it->second.get()).second) {
					// we first see this library
					for (const auto &s: it->second->rewritten_args)
						arg2.push_back(s);
				}
			} else {
				arg2.push_back(arguments[i]);
			}
		}
		return arg2;
	}

	void execute_real_linker() {
		auto arg2 = changesRequired() ? build_linker_argument_list() : arguments;

		// new thing: ld <args> nullptr (as char* array)
		std::vector<char *> argv(arg2.size() + 2);
		auto linker = "/usr/bin/ld.lld-7";
		argv[0] = const_cast<char *>(linker);
		for (size_t i = 0; i < arg2.size(); i++) {
			argv[i + 1] = const_cast<char *>(arg2[i].c_str());
		}
		argv[arg2.size() + 1] = nullptr;
		cout << "Executing real linker... " << endl;

		std::cerr << "> " << linker;
		for (auto &s: arg2) std::cerr << " " << s;
		std::cerr << std::endl;

		execv(linker, argv.data());
		perror("execv failed");
	}

};

void print_cmdline(int argc, const char *argv[]) {
	std::cerr << "----- CALI INVOCATION: -----\n";
	for (int i = 0; i < argc; i++) {
		if (i != 0) std::cerr << " ";
		std::string s(argv[i]);
		if (s.find(' ') != s.npos) std::cerr << "\"" << s << "\"";
		else std::cerr << s;
	}
	std::cerr << "\nWorkdir: " << filesystem::current_path().string();
	std::cerr << "\n----- END CALI INVOCATION -----" << std::endl;
}

int main(int argc, const char *argv[]) {
	print_cmdline(argc, argv);
	applicationPath = filesystem::absolute(argv[0]);
	while (filesystem::is_symlink(applicationPath)) {
		applicationPath = filesystem::read_symlink(applicationPath);
	}
	applicationPath = applicationPath.parent_path();

	auto linker = LinkerArguments(argc, argv);
	linker.link();
	return 0;
}