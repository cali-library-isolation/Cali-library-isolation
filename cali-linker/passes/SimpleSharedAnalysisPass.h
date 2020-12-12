#ifndef LLVMREWRITER_SIMPLESHAREDANALYSISPASS_H
#define LLVMREWRITER_SIMPLESHAREDANALYSISPASS_H

#include <llvm/Analysis/CallGraphSCCPass.h>
#include "PDG.h"
#include "DataDrivenSCCPass.h"
#include "../cali_linker/configfile.h"

namespace basegraph {
	class ColorMap;
}

namespace llvm {
	class CallInst;
}


class SimpleSharedAnalysisPass : public llvm::ModulePass {
	PDG *pdg = nullptr;
	const YamlConfig *config = nullptr;

public:
	static char ID;

	explicit SimpleSharedAnalysisPass(const YamlConfig *config) : llvm::ModulePass(ID), config(config) {}

	SimpleSharedAnalysisPass() : llvm::ModulePass(ID) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;

	void reportSharedFunctionPointer(Vertex v);

};

#endif //LLVMREWRITER_SIMPLESHAREDANALYSISPASS_H
