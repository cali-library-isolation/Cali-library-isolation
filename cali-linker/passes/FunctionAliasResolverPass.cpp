#include "FunctionAliasResolverPass.h"
#include <llvm/IR/Module.h>

char FunctionAliasResolverPass::ID = 0;

bool FunctionAliasResolverPass::runOnModule(llvm::Module &M) {
	int count = 0;
	auto allAliasCount = M.getAliasList().size();

	for (auto &alias: M.getAliasList()) {
		if (alias.getNumUses() > 0 && alias.getType()->isPointerTy() && alias.getType()->getPointerElementType()->isFunctionTy()) {
			alias.replaceAllUsesWith(alias.getAliasee());
			count++;
		}
	}

	llvm::outs() << "[FunctionAliasResolverPass] resolved " << count << " / " << allAliasCount << " aliases\n";

	return count > 0;
}
