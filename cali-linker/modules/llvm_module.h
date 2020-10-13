#ifndef LLVMREWRITER_LLVM_MODULE_H
#define LLVMREWRITER_LLVM_MODULE_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include "ipc_module.h"


namespace ipcrewriter {

	class LlvmIpcModule : public IpcModule {
	private:
		std::unique_ptr<llvm::Module> module;
		int next_coverage_id = 0;
	protected:
		void loadModuleFile() override;

		void runAnalysisTodo(const std::set<std::string> &symbols);

		llvm::Function *instrumentCoverageReplacement(llvm::Function *function, std::string description);

		void writeConfiguration();

		void preventInlining();
		void handleFreeFunction(const std::string& name);

	public:
		static llvm::LLVMContext context;

		bool canRewrite() override {
			return true;
		}

		void rewriteFunctionCalls(CommunicationPair &cmp, const std::set<std::string> &symbols) override;

		void save() override;

		std::vector<std::string> rewrittenFilename() override;

	public:
		LlvmIpcModule(const std::string &source, bool mainModule, YamlConfig *config, const ContextConfig *contextConfig);

		LlvmIpcModule(std::unique_ptr<llvm::Module> module, bool mainModule, YamlConfig *config, const ContextConfig *contextConfig)
				: IpcModule(module->getName(), mainModule, config, contextConfig), module(std::move(module)) {}

	};

}

#endif //LLVMREWRITER_LLVM_MODULE_H
