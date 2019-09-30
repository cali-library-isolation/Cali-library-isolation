#ifndef LLVMREWRITER_PDGWRITERPASS_H
#define LLVMREWRITER_PDGWRITERPASS_H


#include <llvm/Pass.h>

class PDGWriterPass : public llvm::ModulePass {
public:
	static char ID;
	bool runDot = false;

	PDGWriterPass() : llvm::ModulePass(ID) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;


};


#endif //LLVMREWRITER_PDGWRITERPASS_H
