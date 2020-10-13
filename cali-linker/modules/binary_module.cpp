#include "ipc_module.h"
#include <iostream>
#include <sstream>

namespace ipcrewriter {

	using namespace std;

	static std::string exec(std::string cmd) {
		std::array<char, 128> buffer;
		std::string result;
		std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
		if (!pipe) throw std::runtime_error("popen() failed!");
		while (!feof(pipe.get())) {
			if (fgets(buffer.data(), 128, pipe.get()) != NULL)
				result += buffer.data();
		}
		return result;
	}

	BinaryIpcModule::BinaryIpcModule(const std::string &source, bool mainModule, YamlConfig *config, const ContextConfig *contextConfig, bool isMixedArchive)
			: IpcModule(source, mainModule, config, contextConfig) {
		if (isMixedArchive) {
			rewrittenFilenames.push_back(source);
		}
	}

	void BinaryIpcModule::loadModuleFile() {
		// execute "nm -gB '<filename>'"
		cerr << "> nm -gDB '" + source + "'" << endl;
		string result = exec("LANG=C.UTF-8 LANGUAGE=en_GB nm -gDB '" + source + "'");
		istringstream iss(result);
		// Command output (export / import):
		// 0000000000201038 B xxx
		//                  w yyy
		for (std::string line; std::getline(iss, line);) {
			if (!line.empty() && line[line.size()-1] == ':' && line.find(' ') == std::string::npos) continue;
			bool isImport = line[0] == ' ';
			unsigned int i;
			for (i = 1; i < line.length(); i++) {
				if (line[i - 1] == ' ' && line[i] != ' ') {
					// line[i] is type (B/w/D/T/U/...) -> line[i+2] is symbol name
					auto s = line.substr(i + 2);
					auto &ignoredFunctions = contextConfig->ignoredFunctions;
					if (contextConfig && std::find(ignoredFunctions.begin(), ignoredFunctions.end(), s) != ignoredFunctions.end())
						break;
					(isImport ? imports : exports).insert(s);
					break;
				}
			}
			if (i == line.length()) throw runtime_error("Can't parse line \"" + line + "\"");
		}
	}

	void BinaryIpcModule::rewriteFunctionCalls(CommunicationPair &cmp, const std::set<std::string> &symbols) {
		throw "NI";
	}

	void BinaryIpcModule::save() {
		throw "NI";
	}

}
