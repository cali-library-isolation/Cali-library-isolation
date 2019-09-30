#ifndef LLVMREWRITER_IPC_MODULE_H
#define LLVMREWRITER_IPC_MODULE_H

#include <string>
#include <set>
#include <memory>
#include "../cali_linker/ipc_rewriter.h"
#include "communication_pair.h"
#include "../cali_linker/configfile.h"

namespace ipcrewriter {

	class IpcModule {
	protected:
		YamlConfig* config;
		const ContextConfig* contextConfig;
		bool mainModule = false;
		std::string source;
		std::set<std::string> imports;
		std::set<std::string> exports;

	public:
		IpcModule(const std::string &source, bool mainModule, YamlConfig* config, const ContextConfig* contextConfig)
			: mainModule(mainModule), source(source), config(config), contextConfig(contextConfig) {}

		virtual void loadModuleFile() = 0;

		virtual bool canRewrite() = 0;

		virtual void rewriteFunctionCalls(CommunicationPair &cmp, const std::set<std::string> &symbols) = 0;

		virtual void save() = 0;

		virtual std::vector<std::string> rewrittenFilename() = 0;

		bool isMainModule() const {
			return mainModule;
		}

		const std::string &getSource() const;

		const std::set<std::string> &getImports() const;

		const std::set<std::string> &getExports() const;

		static std::shared_ptr<IpcModule> newIpcModuleFromFile(const std::string &filename, bool isMainModule, YamlConfig* config, const ContextConfig* contextConfig);

		std::set<std::string> ignored; // stdlib etc
	};




	class BinaryIpcModule : public IpcModule {
	public:
		BinaryIpcModule(const std::string &source, bool mainModule, YamlConfig* config, const ContextConfig* contextConfig);

		void loadModuleFile() override;

		bool canRewrite() override {
			return false;
		}

		std::vector<std::string> rewrittenFilename() override {
			return {};
		}

		void rewriteFunctionCalls(CommunicationPair &cmp, const std::set<std::string> &symbols) override;

		void save() override;
	};


	class CompositeIpcModule : public IpcModule {
		std::vector<std::shared_ptr<IpcModule>> modules;
	public:
		CompositeIpcModule(bool isMainModule, YamlConfig* config, const ContextConfig* contextConfig);

		static std::shared_ptr<IpcModule> newIpcModulesFromFiles(std::vector<std::string> &files, bool isMainModule, YamlConfig* config, const ContextConfig* contextConfig);

		inline void add(std::shared_ptr<IpcModule> module) {
			modules.push_back(module);
		}

		void loadModuleFile() override {
			for (const auto &m: modules) {
				m->loadModuleFile();
				imports.insert(m->getImports().begin(), m->getImports().end());
				exports.insert(m->getExports().begin(), m->getExports().end());
			}
		}

		bool canRewrite() override {
			for (const auto &m: modules)
				if (m->canRewrite())
					return true;
			return false;
		}

		void rewriteFunctionCalls(CommunicationPair &cmp, const std::set<std::string> &symbols) override {
			for (const auto &m: modules)
				if (m->canRewrite())
					m->rewriteFunctionCalls(cmp, symbols);
		}

		void save() override {
			for (const auto &m: modules)
				if (m->canRewrite())
					m->save();
		}

		std::vector<std::string> rewrittenFilename() override {
			std::vector<std::string> result;
			for (const auto &m: modules)
				for (const auto &f: m->rewrittenFilename())
					result.push_back(f);
			return result;
		}
	};

}


#endif //LLVMREWRITER_IPC_MODULE_H
