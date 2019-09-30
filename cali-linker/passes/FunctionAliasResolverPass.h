#ifndef LLVMREWRITER_FUNCTIONALIASRESOLVERPASS_H
#define LLVMREWRITER_FUNCTIONALIASRESOLVERPASS_H


#include <llvm/Pass.h>

class FunctionAliasResolverPass : public llvm::ModulePass {
	static char ID;
public:

	FunctionAliasResolverPass() : llvm::ModulePass(ID) {}

	bool runOnModule(llvm::Module &M) override;

};


#endif //LLVMREWRITER_FUNCTIONALIASRESOLVERPASS_H
