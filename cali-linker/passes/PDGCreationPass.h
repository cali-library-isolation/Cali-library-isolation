#ifndef LLVMREWRITER_PDGCREATIONPASS_H
#define LLVMREWRITER_PDGCREATIONPASS_H

#include <llvm/Pass.h>
#include "PDG.h"
#include "../cali_linker/configfile.h"


class PDGCreationPass : public llvm::ModulePass {
public:
	static char ID;

	PDG graph;
	const std::set<std::string> &externalSymbols;
	const YamlConfig *config = nullptr;

	PDGCreationPass(const YamlConfig* config, const std::set<std::string> &externalSymbols)
	: ModulePass(ID), externalSymbols(externalSymbols), config(config), graph(config) {}

	PDGCreationPass() : ModulePass(ID), externalSymbols(*new std::set<std::string>()), graph(nullptr) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;

	/**
	 * Create all necessary edges towards this node (and its subtree).
	 * @param v
	 * @param instruction the llvm::Instruction or llvm::Operator that matches this vertex
	 */
	void addEdges(Vertex v, llvm::User *instruction);

	void specialGEPHandling(llvm::User *ins, Vertex memVertex);

	void analyzeStructs(llvm::Module& M);
};


#endif //LLVMREWRITER_PDGCREATIONPASS_H
