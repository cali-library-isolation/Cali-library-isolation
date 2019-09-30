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
	llvm::Constant *shm_free = nullptr;
	llvm::Constant *shm_posix_memalign = nullptr;

	PDG *pdg = nullptr;
	YamlConfig *config;
	const std::set<std::string> &externalSymbols;
public:
	static char ID;

	PDGSharedMemoryPass(YamlConfig *config, const std::set<std::string> &externalSymbols)
			: llvm::ModulePass(ID), config(config), externalSymbols(externalSymbols) {}

	PDGSharedMemoryPass()
			: llvm::ModulePass(ID), config(nullptr), externalSymbols(*new std::set<std::string>()) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;

	bool shouldBeShared(llvm::Value *def);

	void addSharedMemoryFunctions(llvm::Module &M);

	void makeAllocCallShared(llvm::CallInst &call);

	llvm::Value *makeStackAllocaShared(llvm::AllocaInst &ins);

	void makeGlobalVariablesShared(llvm::Module &M, std::vector<llvm::GlobalVariable *> &vars);

	void checkForFileDescriptorParameters(llvm::CallInst &call);
};


#endif //LLVMREWRITER_PDGSHAREDMEMORYPASS_H
