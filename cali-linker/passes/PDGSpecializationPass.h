#ifndef LLVMREWRITER_PDGSPECIALIZATIONPASS_H
#define LLVMREWRITER_PDGSPECIALIZATIONPASS_H

#include <llvm/Analysis/CallGraphSCCPass.h>
#include <llvm/Transforms/Utils/ValueMapper.h>
#include "PDG.h"
#include "DataDrivenSCCPass.h"
#include "../cali_linker/configfile.h"

namespace llvm {
	class CallInst;
}


class PDGSpecializationPass : public llvm::ModulePass {
	PDG *pdg = nullptr;
	DataDrivenSCCPass *sccpass = nullptr;
	const YamlConfig *config;

public:
	static char ID;

	PDGSpecializationPass() : llvm::ModulePass(ID) {}

	explicit PDGSpecializationPass(const YamlConfig *config) : llvm::ModulePass(ID), config(config) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;


protected:
	std::string getSpecializedName(llvm::StringRef oldName);

	bool isSpecializationRequired(const std::vector<llvm::Function *> &CG);

	void cloneSCC(const std::vector<llvm::Function *> &CG, std::map<llvm::Function *, llvm::Function *> &old_to_new, std::set<llvm::Function *> &new_functions);

	void cloneGraphForSCCFunction(llvm::Function *oldFunction, llvm::Function *newFunction, llvm::ValueToValueMapTy &value_map,
								  std::map<Vertex, Vertex> &vertex_map);

	void cloneGraphForSCCEdges(std::map<Vertex, Vertex> &vertex_map);

	void specializeCalls(llvm::Function *oldFunction, llvm::Function *newFunction);

	bool removeIfDead(llvm::Function *function);

	void propagateFileDescriptors(const std::vector<llvm::Function *> &scc);

};

#endif //LLVMREWRITER_PDGSPECIALIZATIONPASS_H
