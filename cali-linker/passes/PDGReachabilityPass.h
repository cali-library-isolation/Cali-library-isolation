#ifndef LLVMREWRITER_PDGREACHABILITYPASS_H
#define LLVMREWRITER_PDGREACHABILITYPASS_H

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


class PDGReachabilityPass : public llvm::ModulePass {
	PDG *pdg = nullptr;
	const YamlConfig *config = nullptr;
	DataDrivenSCCPass *sccpass = nullptr;
	std::shared_ptr<basegraph::ColorMap> colors;

	template<class InstTy>
	void handleCall(std::vector<InstTy *> &indirectCalls, llvm::Function *function, llvm::Instruction &ins, std::set<llvm::Function *> &functions);

public:
	static char ID;

	explicit PDGReachabilityPass(const YamlConfig *config) : llvm::ModulePass(ID), config(config) {}

	PDGReachabilityPass() : llvm::ModulePass(ID) {}

	void getAnalysisUsage(llvm::AnalysisUsage &Info) const override;

	bool runOnModule(llvm::Module &M) override;

	void runOnSCC(const std::vector<llvm::Function *> &scc);

	void runOnGlobal();

	template<class InstTy>
	bool addKnownFunctionSummary(llvm::Function *calledFunction, InstTy &call);

	template<class InstTy>
	void calculateIpcFunctionCalls(std::vector<InstTy *> indirectCalls);

	/**
	 * Mark all nodes in these functions/SCC that can reach an IPC sink.
	 * @param functions
	 * @return number of initial IPC sinks this cluster contains. If == 0, then no sink is reachable.
	 */
	// size_t calculateSinkReachability(std::set<llvm::Function *> &functions);

	void calculateSummaryEdges(std::set<llvm::Function *> &functions);

	void collectFunctionSignatureNodes(llvm::Function *f, std::set<Vertex> &nodes);

	void collectSubnodesForFunction(Vertex v, std::set<Vertex> &nodes, llvm::Function *f);

	/**
	 * Create a map between two structurally-equal subnode trees.
	 * @param map a map to be filled
	 * @param key
	 * @param value
	 */
	void matchSubnodes(std::map<Vertex, Vertex> &map, Vertex key, Vertex value);

	void markSubnodesReachingIpc(Vertex root);

	template<class InstTy>
	void markIpcCall(InstTy *call);

};

#endif //LLVMREWRITER_PDGREACHABILITYPASS_H
