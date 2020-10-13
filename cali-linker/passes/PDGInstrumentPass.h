#ifndef LLVMREWRITER_PDGINSTRUMENTPASS_H
#define LLVMREWRITER_PDGINSTRUMENTPASS_H


#include <set>
#include <llvm/Pass.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>

class PDG;

class PDGInstrumentPass : public llvm::ModulePass {
private:
	llvm::ConstantInt *getInt64(llvm::Module &M, uint64_t value);

	void instrumentAllocation(llvm::Module &M, PDG &pdg, uint64_t id, llvm::Value *instruction);

	std::vector<llvm::ReturnInst *> &getReturns(llvm::Function *function);

	std::map<llvm::Function *, std::vector<llvm::ReturnInst *>> functionReturns;

	std::set<std::string> &ignored;

public:
	static char ID;

	explicit PDGInstrumentPass(std::set<std::string> &ignored) : ModulePass(ID), ignored(ignored) {}

	void prepareStubs(llvm::Module &module);

	void getAnalysisUsage(llvm::AnalysisUsage &usage) const override;

	bool runOnModule(llvm::Module &M) override;

};


#endif //LLVMREWRITER_PDGINSTRUMENTPASS_H
