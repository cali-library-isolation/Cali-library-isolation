#ifndef LLVMREWRITER_DATADRIVENSCCPASS_H
#define LLVMREWRITER_DATADRIVENSCCPASS_H

#include <llvm/Analysis/CallGraphSCCPass.h>
#include <llvm/IR/Instructions.h>

class DataDrivenSCCPass : public llvm::ModulePass {

	std::vector<std::vector<llvm::Function *>> dataSCCs; // order: callee to caller
	int maxSccSize = 0;

public:
	static char ID;

	DataDrivenSCCPass() : llvm::ModulePass(ID) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;

	template <class InstTy>
	static bool considerCall(llvm::CallBase<InstTy> *call);

	inline const std::vector<std::vector<llvm::Function * >> &getSCCs() {
		return dataSCCs;
	}
};

#endif //LLVMREWRITER_DATADRIVENSCCPASS_H
