#ifndef LLVMREWRITER_DATADRIVENSCCPASS_H
#define LLVMREWRITER_DATADRIVENSCCPASS_H

#include <llvm/IR/Instructions.h>
#include <set>
#include <llvm/PassAnalysisSupport.h>
#include "../cali_linker/configfile.h"

class DataDrivenSCCPass : public llvm::ModulePass {

	const YamlConfig *config = nullptr;
	const ContextConfig *contextConfig = nullptr;

	std::vector<std::vector<llvm::Function *>> dataSCCs; // order: callee to caller
	int maxSccSize = 0;

	// Artifacts from struct analysis
	std::map<const llvm::StructType *, int> structClusters;

	void analyzeStructs(llvm::Module &M);

	// Artifacts from FP analysis
	std::vector<llvm::Function *> possibleIndirectCallTargets;
	std::map<llvm::Instruction *, std::shared_ptr<std::vector<llvm::Function *>>> possibleInstructionTargets;
	std::map<llvm::Function *, std::shared_ptr<std::vector<llvm::Instruction *>>> possibleFunctionCalls;

public:
	static char ID;

	DataDrivenSCCPass() : llvm::ModulePass(ID), config(nullptr) {}

	DataDrivenSCCPass(const YamlConfig *config, const ContextConfig *contextConfig) : llvm::ModulePass(ID), config(config), contextConfig(contextConfig) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;

	template<class InstTy>
	static bool considerCall(llvm::CallBase<InstTy> *call, bool needsFunction = true);

	bool considerFunction(llvm::Function *function);

	template<class InstTy>
	std::shared_ptr<const std::vector<llvm::Function *>> getPossibleIndirectFunctions(llvm::CallBase<InstTy> *call);

	std::shared_ptr<const std::vector<llvm::Instruction *>> getPossibleIndirectCalls(llvm::Function *function);

	bool isCompatibleCallType(llvm::FunctionType *callType, llvm::FunctionType *functionType);

	inline const std::vector<std::vector<llvm::Function * >> &getSCCs() {
		return dataSCCs;
	}

	inline const std::map<const llvm::StructType *, int> &getStructClusters() {
		return structClusters;
	}
};

#endif //LLVMREWRITER_DATADRIVENSCCPASS_H
