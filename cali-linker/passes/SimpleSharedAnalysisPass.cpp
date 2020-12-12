#include <iostream>
#include <chrono>
#include <boost/graph/breadth_first_search.hpp>
#include "SimpleSharedAnalysisPass.h"
#include "PDGCreationPass.h"
#include "../graph/algorithms.h"
#include "PDGUtilities.h"
#include "../cali_linker/debug.h"
#include <llvm/IR/Constants.h>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/Support/Format.h>

using namespace llvm;
using namespace boost;


char SimpleSharedAnalysisPass::ID = 0;

static RegisterPass<SimpleSharedAnalysisPass> reg("pdg-simplesharedanalysis", "PDG data flow calculation", false, true);

void SimpleSharedAnalysisPass::getAnalysisUsage(AnalysisUsage &Info) const {
	Info.setPreservesCFG();
	Info.setPreservesAll();
	Info.addRequired<PDGCreationPass>();
}

bool SimpleSharedAnalysisPass::runOnModule(Module &M) {
	pdg = &getAnalysis<PDGCreationPass>().graph;
	auto &graph = *pdg;

	llvm::errs() << "----- Checking for shared function pointers ... -----\n";

	for (auto v: pdg->vertex_set()) {
		if (!graph[v].ipcsink && !graph[v].reaches_ipc_sink) continue;
		auto t = graph[v].type;
		if (!t) continue;
		if ((t->isPointerTy() && t->getPointerElementType()->isFunctionTy()) || t->isFunctionTy()) {
			reportSharedFunctionPointer(v);
		}
	}

	llvm::errs() << "----- Check finished -----\n";

	return false;
}

void SimpleSharedAnalysisPass::reportSharedFunctionPointer(Vertex v) {
	auto &graph = *pdg;
	llvm::errs() << "[WARN] Shared function pointer in vertex " << v << "\n";
	if (graph[v].function)
		llvm::errs() << "       Function:    " << graph[v].function->getName() << "\n";
	if (graph[v].instruction)
		llvm::errs() << "       Instruction: " << graph[v].instruction->getName() << "\n";
	llvm::errs() << "       Type:        " << *graph[v].type << "\n";

	for (auto e: graph.in_edges(v)) {
		if (e.property.type == PDGEdge::PDGEdgeType::Deref || e.property.type == PDGEdge::PDGEdgeType::Part) {
			llvm::errs() << "     - Referenced by: " << e.source << "\n";
			if (graph[e.source].function)
				llvm::errs() << "       Function:    " << graph[e.source].function->getName() << "\n";
			if (graph[e.source].instruction)
				llvm::errs() << "       Instruction: " << graph[e.source].instruction->getName() << "\n";
			llvm::errs() << "       Type:        " << *graph[e.source].type << "\n";
		}
	}
}


