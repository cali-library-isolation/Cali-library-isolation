#ifndef LLVMREWRITER_IPC_REWRITER_H
#define LLVMREWRITER_IPC_REWRITER_H


#include <vector>
#include <memory>
#include "configfile.h"

namespace ipcrewriter {

	class IpcModule;
	class CommunicationPair;

	class IpcRewriter {
	private:
		std::vector<std::shared_ptr<IpcModule>> modules;
		std::vector<std::shared_ptr<CommunicationPair>> communications;

	public:
		std::vector<std::string> new_files;
		std::vector<std::string> new_arguments;

		void addModule(std::shared_ptr<IpcModule> module);

		void createIpcJail(const ContextConfig* permissions, const YamlConfig* config);

		void createCommunicationPair(IpcModule& m1, IpcModule& m2, const ContextConfig* permissions, const YamlConfig* config);

		void saveAllModules(const std::string &basepath);

		void saveLogEntries(const std::string &fname);
	};

}


#endif //LLVMREWRITER_IPC_REWRITER_H
