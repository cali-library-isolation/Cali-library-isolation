#ifndef LLVMREWRITER_PDGSHAREDMEMORYPASS_H
#define LLVMREWRITER_PDGSHAREDMEMORYPASS_H


#include <llvm/Pass.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Instructions.h>
#include "PDG.h"
#include "../cali_linker/configfile.h"

class PDGSharedMemoryPass : public llvm::ModulePass {
	llvm::Constant *shm_malloc = nullptr;
	llvm::Constant *shm_calloc = nullptr;
	llvm::Constant *shm_realloc = nullptr;
	llvm::Constant *shm_free = nullptr;
	llvm::Constant *shm_posix_memalign = nullptr;
	llvm::Constant *shm_mmap = nullptr;

	PDG *pdg = nullptr;
	YamlConfig *config;
	const ContextConfig* contextConfig;
	const std::set<std::string> &externalSymbols;
	llvm::Constant* getSharedMemoryReplacementFunc(llvm::Function* function, const std::string& name);

public:
	static char ID;

	PDGSharedMemoryPass(YamlConfig *config, const ContextConfig* contextConfig, const std::set<std::string> &externalSymbols)
			: llvm::ModulePass(ID), config(config), contextConfig(contextConfig), externalSymbols(externalSymbols) {}

	PDGSharedMemoryPass() : llvm::ModulePass(ID), config(nullptr), externalSymbols(*new std::set<std::string>()), contextConfig(nullptr) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;

	bool shouldBeShared(llvm::Value *def);

	void addSharedMemoryFunctions(llvm::Module &M);

	llvm::Constant* getSharedMemoryFunction(llvm::Function* allocFunction);

	void makeAllocCallShared(llvm::CallInst &call);

	llvm::Value *makeStackAllocaShared(llvm::AllocaInst &ins);

	void makeGlobalVariablesShared(llvm::Module &M, std::vector<llvm::GlobalVariable *> &vars);

	void checkForFileDescriptorParameters(llvm::CallInst &call);

	void makeIndirectFunctionsShared(llvm::Module &M);
};


#endif //LLVMREWRITER_PDGSHAREDMEMORYPASS_H
