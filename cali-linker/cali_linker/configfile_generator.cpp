#include <boost/optional.hpp>
#include "configfile_generator.h"
#include "../stdlib-is-shit.h"
#include <sstream>
#include <set>
#include <map>
#include <boost/filesystem.hpp>
#include <iostream>

using namespace std;
using namespace boost;


namespace {
	const std::set<string> stdlib{"libc.so", "libgcc_eh.a", "libgcc.a", "libgcc_s.a", "libgcc_s.so", "libpthread.so", "libm.so", "libdl.so", "librt.so"};

	struct PrecompiledLibrary {
		std::string global;
		std::string main;
		std::string library;
	};

	const std::map<string, PrecompiledLibrary> precompiledLibraries{
			{
					"-lgcrypt",
					{
							"replaceArguments:  # for libgcrypt\n"
							"  \"-lgcrypt\": [\"/usr/cali-lib/libgcrypt.bc\", \"/usr/cali-lib/libgcrypt-elf.a\"]\n",
							"    function_behavior:  # for libgcrypt\n"
							"      gcry_xmalloc: malloc\n"
							"      gcry_xcalloc: calloc\n"
							"      gcry_xrealloc: realloc\n"
							"      gcry_xstrdup: strdup\n"
							"      gcry_free: free\n"
							"      gcry_malloc: malloc\n"
							"      gcry_calloc: calloc\n"
							"      gcry_realloc: realloc\n"
							"      gcry_strdup: strdup\n"
							"      gpgrt_asprintf: asprintf\n"
							"      gpgrt_vasprintf: vasprintf\n"
							"      gcry_malloc_secure: malloc\n"
							"      gcry_calloc_secure: calloc\n"
							"      gcry_xmalloc_secure: malloc\n"
							"      gcry_xcalloc_secure: calloc\n"
							"      _gcry_xmalloc: malloc\n"
							"      _gcry_xcalloc: calloc\n"
							"      _gcry_xrealloc: realloc\n"
							"      _gcry_xstrdup: strdup\n"
							"      _gcry_free: free\n"
							"      _gcry_malloc: malloc\n"
							"      _gcry_calloc: calloc\n"
							"      _gcry_realloc: realloc\n"
							"      _gcry_strdup: strdup\n"
							"      _gpgrt_asprintf: asprintf\n"
							"      _gpgrt_vasprintf: vasprintf\n"
							"      _gcry_malloc_secure: malloc\n"
							"      _gcry_calloc_secure: calloc\n"
							"      _gcry_xmalloc_secure: malloc\n"
							"      _gcry_xcalloc_secure: calloc\n",
							"    ignored_functions:  # for libgcrypt\n"
							"    - gcry_xmalloc\n"
							"    - gcry_xcalloc\n"
							"    - gcry_xrealloc\n"
							"    - gcry_xstrdup\n"
							"    - gcry_free\n"
							"    - gcry_malloc\n"
							"    - gcry_calloc\n"
							"    - gcry_realloc\n"
							"    - gcry_strdup\n"
					}
			}
	};

	boost::optional<string> find_library(const string &name, vector<string> &librarySearchpath) {
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
		return {};
	}

	struct GeneratedConfig {
		std::string config;
		size_t numLibrarySelectors;
	};

	GeneratedConfig internalGenerateBoilerplateConfiguration(std::vector<std::string> &arguments) {
		std::string output_filename;
		std::vector<std::string> librarySearchpath;

		// parse commandline, part 1
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
				return {"This build config is creating a shared library.", 0};
			}
		}

		// Parse commandline, part 2
		bool suggestLibStdCxx = false;
		set<string> mainSelectors;
		set<string> librarySelectors;
		set<string> replaceArguments;
		PrecompiledLibrary modifications;
		for (size_t i = 0; i < arguments.size(); i++) {
			const auto &s = arguments[i];
			if (s == "-lstdc++") {
				suggestLibStdCxx = true;
				mainSelectors.insert("libstdc++.bc");
			} else if (startsWith(s, "-l") || endsWith(s, ".so") || endsWith(s, ".a")) {
				auto it = precompiledLibraries.find(s);
				if (it != precompiledLibraries.end()) {
					modifications.global += it->second.global;
					modifications.main += it->second.main;
					modifications.library += it->second.library;
					continue;
				}
				if (startsWith(s, "-l")) {
					auto libname = "/usr/cali-lib/lib" + s.substr(2) + ".a";
					if (filesystem::exists(libname)) {
						replaceArguments.insert("\"" + s + "\": [\"" + libname + "\"]");
						mainSelectors.insert("lib" + s.substr(2) + ".a");
						continue;
					}
				}

				// library file? -l/.so likely, .a questionable
				auto lib = find_library(startsWith(s, "-l") ? "lib" + s.substr(2) : s, librarySearchpath);
				if (lib) {
					if (endsWith(*lib, ".so") || startsWith(*lib, "/usr/") || startsWith(*lib, "/lib")) {
						// add library to candidates
						int p = lib->rfind('/');
						if (p != string::npos) librarySelectors.insert(lib->substr(p + 1));
						else librarySelectors.insert(*lib);
					} else if (endsWith(*lib, ".a")) {
						// add static library to main code
						int p = lib->rfind('/');
						if (lib->find("/.libs/") != string::npos) mainSelectors.insert(".libs/*.a");
						else if (p != string::npos) mainSelectors.insert("^" + lib->substr(0, p) + "/*.a");
						else mainSelectors.insert(*lib);
					}
				}
			} else if ((endsWith(s, ".o") || endsWith(s, ".bc") || endsWith(s, ".lo")) && !startsWith(s, "/usr/")) {
				// local object file, should go to "main"
				int p = s.rfind('/');
				int p2 = s.rfind('.');
				if (p != string::npos) mainSelectors.insert(s.substr(0, p) + "/*." + s.substr(p2 + 1));
				else mainSelectors.insert("*." + s.substr(p2 + 1));
			} else if (s == "-o" || s == "-plugin" || s == "-rpath") {
				i++;
			}
		}

		stringstream ss;
		ss << "---" << endl;
		ss << endl;
		ss << "addLibstdc++: " << (suggestLibStdCxx ? "true" : "false") << endl;
		ss << "strongFPAnalysis: false  # try to change if program crashes" << endl;
		ss << "strongFDAnalysis: false  # try to change if program crashes" << endl;
		ss << "linkerOverride: " << (suggestLibStdCxx ? "false" : "true") << endl; // C++ programs might have the "undefined symbol thunk" problem
		if (replaceArguments.empty()) {
			ss << "replaceArguments: {}" << endl;
		} else {
			ss << "replaceArguments:" << endl;
			for (const auto &s: replaceArguments) ss << "  " << s << "\n";
		}
		ss << modifications.global;
		ss << endl;
		ss << "contexts:" << endl;
		ss << "  main:" << endl;
		ss << "    mprotect_mode: false" << endl;
		ss << "    sequential_mode: false" << endl;
		ss
				<< "    # programIsForking: true                # try to uncomment if things break because the program is using non-trivial forks (typical symptom: hanging)"
				<< endl;
		ss << "    # concurrentLibraryCommunication: true  # try to uncomment if things break" << endl;
		ss << "    # silent: 1                             # try to uncomment if things break (with unexpected output etc)" << endl;
		ss << "    selectors:" << endl;
		for (auto &selector: mainSelectors) {
			ss << "      - \"" << selector << "\"" << endl;
		}
		ss << modifications.main;
		ss << "  library:" << endl;
		ss << "    selectors:  # TODO uncomment libraries you want to isolate" << endl;
		for (auto &selector: librarySelectors) {
			if (stdlib.find(selector) != stdlib.end()) continue;
			ss << "      # - " << selector << endl;
		}
		ss << "    permissions:  # TODO configure" << endl;
		ss << "      readonly:" << endl;
		ss << "        # - /" << endl;
		ss << "        # - \"$CWD\"" << endl;
		ss << "        # - /tmp" << endl;
		ss << "        # - /dev/urandom" << endl;
		ss << "        - /boot" << endl;
		ss << "      readwrite:" << endl;
		ss << "        # - \"$ARGV_FOLDERS\"" << endl;
		ss << "        # - \"/tmp\"" << endl;
		ss << "        - /" << endl;
		ss << "      network: all  # all, local, none" << endl;
		ss << modifications.library;
		return {ss.str(), librarySelectors.size()};
	}
}

std::string generateBoilerplateConfiguration(std::vector<std::string> &arguments) {
	return internalGenerateBoilerplateConfiguration(arguments).config;
}

void writeBoilerplateConfiguration(std::vector<std::string> &arguments, const std::string &filename) {
	auto config = internalGenerateBoilerplateConfiguration(arguments);
	if (config.numLibrarySelectors > 0) {
		int num = 0;
		while (filesystem::exists(filename + ".tmpl." + to_string(num))) {
			ifstream f(filename + ".tmpl." + to_string(num));
			stringstream ss;
			ss << f.rdbuf();
			if (ss.str() == config.config)
				return;
			num++;
		}
		ofstream f(filename + ".tmpl." + to_string(num));
		f << config.config;
		f.close();
		cerr << "Wrote template configuration \"" << filename + ".tmpl." + to_string(num) << "\".\n";
	}
}
