#include <iostream>
#include "PDGCreationPass.h"
#include "PDGReachabilityPass.h"
#include "PDGSpecializationPass.h"
#include "PDGSharedReachabilityPass.h"
#include "../graph/algorithms.h"
#include "PDGUtilities.h"
#include "../cali_linker/debug.h"
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/Analysis/CallGraph.h>


using namespace llvm;


namespace {
	size_t calculateReachability(PDG &graph, std::set<Function *> &functions, basegraph::ColorMap &colors) {
		struct ForwardDataReachability : public basegraph::default_bfs_visitor<PDG> {
			std::set<Function *> &functions;

			explicit ForwardDataReachability(std::set<Function *> &functions) : functions(functions) {}

			bool examine_forwards_edge(Edge e, const PDG &graph) {
				auto t = graph[e].type;
				return t == PDGEdge::Data || t == PDGEdge::Part || t == PDGEdge::Deref || t == PDGEdge::Summary_Data;
			}

			bool examine_backwards_edge(Edge e, const PDG &graph) {
				return false;
			}

			bool discover_vertex(Vertex v, PDG &graph) {
				graph[v].influenced_by_shared = true;
				auto f = graph[v].function;
				return !f || functions.find(f) != functions.end();
			}
		};
		std::vector<Vertex> sources{};
		// Default propagation
		for (auto f: functions) {
			for (auto v: graph.getVerticesForFunction(f)) {
				if (graph[v].function != f) continue;
				// throw std::runtime_error("Error in function vertex index");
				if (graph[v].ipcsink || graph[v].reaches_ipc_sink || graph[v].influenced_by_shared)
					sources.push_back(v);
			}
		}
		breadth_first_search_forward(graph, ForwardDataReachability(functions), sources, colors);
		return sources.size();
	}
}



char PDGSharedReachabilityPass::ID = 0;

static RegisterPass<PDGSharedReachabilityPass> reg("pdg-shared-reachability",
												   "PDG shared reachability computation - which value might be influenced by shared memory?", false, false);


void PDGSharedReachabilityPass::getAnalysisUsage(llvm::AnalysisUsage &Info) const {
	Info.addRequired<PDGCreationPass>();
	Info.addRequired<PDGReachabilityPass>();
	Info.addRequired<PDGSpecializationPass>();
	Info.addRequired<DataDrivenSCCPass>();
	Info.addPreserved<PDGCreationPass>();
	Info.addPreserved<PDGReachabilityPass>();
	Info.addPreserved<PDGSpecializationPass>();
	Info.addPreserved<DataDrivenSCCPass>();
}

bool PDGSharedReachabilityPass::runOnModule(Module &M) {
	pdg = &getAnalysis<PDGCreationPass>().graph;
	sccpass = &getAnalysis<DataDrivenSCCPass>();
	auto &SCCs = sccpass->getSCCs();
	bool result = false;
	basegraph::ColorMap colors(*pdg);

	outs() << "--- Checking for usages of shared values ---" << "\n";

	for (auto &scc: SCCs) {
		// callee before caller
		std::set<Function*> functions(scc.begin(), scc.end());
		calculateReachability(*pdg, functions, colors);
		propagateInfluenceToCallers(scc);
	}

	for (auto it = SCCs.rbegin(); it != SCCs.rend(); ++it) {
		// caller before callee
		auto &scc = *it;
		std::set<Function*> functions(scc.begin(), scc.end());
		if (propagateInfluenceFromCallers(scc))
			calculateReachability(*pdg, functions, colors);
	}

	warnForPossiblyDangerousActions(M);

	outs() << "--- Checking for usages of shared values finished. ---" << "\n";

	return result;
}


void PDGSharedReachabilityPass::matchSubnodes(std::map<Vertex, Vertex> &map, Vertex key, Vertex value) {
	auto &graph = *pdg;
	auto val2 = map.find(key);
	if (val2 == map.end()) {
		map[key] = value;
		for (const auto &e: graph.out_edges(key)) {
			if (graph[e].type == PDGEdge::Deref) {
				auto v = graph.getDerefOpt(value);
				if (v)
					matchSubnodes(map, e.target, *v);
			} else if (graph[e].type == PDGEdge::Part) {
				auto v = graph.getPartOpt(value, graph[e].index);
				if (v)
					matchSubnodes(map, e.target, *v);
			}
		}
	} else {
		graph.addSubnodeEdges(val2->second, value);
		graph.addSubnodeEdges(value, val2->second);
	}
}

template<class T>
bool PDGSharedReachabilityPass::propagateCallInfluence(T& call, bool isCalleeToCaller) {
	auto function = call.getFunction();
	bool anyMarkPropagated = false;

	auto handleCallTarget = [this, function, &call, isCalleeToCaller, &anyMarkPropagated](Function *calledFunction) {
		auto &graph = *pdg;
		if (calledFunction && isa<Function>(calledFunction)) {
			// Function is in another SCC - there must be summary edges
			auto vCall = graph.getVertex(&call);
			auto vFunc = graph.getVertex(calledFunction);
			// function signature nodes -> callside nodes. We expect structural equivalence (in terms of parameters / return value structure)
			std::map<Vertex, Vertex> matching;
			// params
			for (unsigned int i = 0; i < call.getNumArgOperands(); i++) {
				if (isIgnoredOperand(call.getArgOperand(i))) continue;
				if (calledFunction->isVarArg() && i >= calledFunction->arg_size()) {
					auto vFuncParam = graph.getParamOpt(vFunc, calledFunction->arg_size());
					if (vFuncParam) {
						matchSubnodes(matching, *vFuncParam, graph.getVertex(call.getArgOperand(i), function));
						// hack, since we have no vararg types on callee side
						// TODO direction here (?)
						if (isCalleeToCaller && graph[*vFuncParam].influenced_by_shared) {
							auto &x = graph[graph.getVertex(call.getArgOperand(i), function)];
							if (!x.influenced_by_shared) anyMarkPropagated = true;
							x.influenced_by_shared = true;
						} else if (!isCalleeToCaller && graph[graph.getVertex(call.getArgOperand(i), function)].influenced_by_shared) {
							if (!graph[*vFuncParam].influenced_by_shared) anyMarkPropagated = true;
							graph[*vFuncParam].influenced_by_shared = true;
						}
					}
				} else {
					auto vFuncParam = graph.getParamOpt(vFunc, i);
					if (vFuncParam)
						matchSubnodes(matching, *vFuncParam, graph.getVertex(call.getArgOperand(i), function));
				}
			}
			// return value
			auto vFuncRet = graph.getReturnOpt(vFunc);
			if (vFuncRet)
				matchSubnodes(matching, *vFuncRet, vCall);

			// copy influenced property
			if (isCalleeToCaller) {
				// TODO too much(!)
				for (const auto &it: matching) {
					if (graph[it.first].reaches_ipc_sink || graph[it.first].ipcsink || graph[it.first].influenced_by_shared) {
						if (!graph[it.second].influenced_by_shared) anyMarkPropagated = true;
						graph[it.second].influenced_by_shared = true;
					}
				}
			} else {
				for (const auto &it: matching) {
					if (graph[it.second].reaches_ipc_sink || graph[it.second].ipcsink || graph[it.second].influenced_by_shared) {
						if (!graph[it.first].influenced_by_shared) anyMarkPropagated = true;
						graph[it.first].influenced_by_shared = true;
					}
				}
			}
		}
	};

	// TODO
	auto calledFunction = getCalledFunction(call);
	handleCallTarget(calledFunction);
	return anyMarkPropagated;
}

void PDGSharedReachabilityPass::propagateInfluenceToCallers(const std::vector<llvm::Function *> &functions) {
	for (auto f: functions) {
		if (!considerFunction(f)) continue;
		for (auto user: f->users()) {
			auto call = dyn_cast<CallInst>(user);
			if (call && getCalledFunction(call) == f) {
				propagateCallInfluence(*call, true);
			}
			auto invoke = dyn_cast<InvokeInst>(user);
			if (invoke && getCalledFunction(invoke) == f) {
				propagateCallInfluence(*invoke, true);
			}
		}
	}
}

/**
 *
 * @param functions
 * @return true if any mark has been propagated
 */
bool PDGSharedReachabilityPass::propagateInfluenceFromCallers(const std::vector<llvm::Function *> &functions) {
	auto anyMarkPropagated = false;
	for (auto f: functions) {
		if (!considerFunction(f)) continue;
		for (auto user: f->users()) {
			auto call = dyn_cast<CallInst>(user);
			if (call && getCalledFunction(call) == f) {
				anyMarkPropagated = anyMarkPropagated | propagateCallInfluence(*call, false);
			}
			auto invoke = dyn_cast<InvokeInst>(user);
			if (invoke && getCalledFunction(invoke) == f) {
				anyMarkPropagated = anyMarkPropagated | propagateCallInfluence(*invoke, false);
			}
		}
	}
	return false;
}

bool PDGSharedReachabilityPass::considerFunction(const Function* f) {
	if (f->isDeclaration()) return false;
	llvm::outs() << "consider function " << f->getName() << "?\n";
	if (f->getName() == "free") {
		llvm::outs() << "[EXCLUDED] " << f->getName() << " excluded (2)\n";
		return false;
	}
	auto it = contextConfig->functionBehavior.find(f->getName());
	if (it != contextConfig->functionBehavior.end()) {
		if (it->second == "free") llvm::outs() << "[EXCLUDED] " << f->getName() << " excluded (1)\n";
		if (it->second == "free") return false;
	}
	return true;
}

void PDGSharedReachabilityPass::warnForPossiblyDangerousActions(Module &M) {
	auto &graph = *pdg;
	long warnings = 0;

	for (auto &f: M.functions()) {
		//if (!considerFunction(&f)) continue;
		if (f.isDeclaration()) continue;
		for (auto &bb: f) {
			for (auto &ins: bb) {
				auto call = dyn_cast<CallInst>(&ins);
				if (call) {
					if (!call->isInlineAsm() && !isa<Function>(call->getCalledValue())) {
						auto v = graph.getVertexOpt(call->getCalledValue());
						if (v && graph[*v].influenced_by_shared) {
							llvm::outs() << "[WARN] In function " << f.getName() << ", there is a call to an indirect value influenced by shared memory!\n";
							llvm::outs() << "       Instruction: " << ins << "\n";
							llvm::outs() << "       Graph vertex: " << *v << "\n";
							warnings++;
						}
					}
				} else {
					auto invoke = dyn_cast<InvokeInst>(&ins);
					if (invoke) {
						if (!isa<Function>(invoke->getCalledValue())) {
							auto v = graph.getVertexOpt(invoke->getCalledValue());
							if (v && graph[*v].influenced_by_shared) {
								llvm::outs() << "[WARN] In function " << f.getName() << ", there is an invoke to an indirect value influenced by shared memory!\n";
								llvm::outs() << "       Instruction: " << ins << "\n";
								llvm::outs() << "       Graph vertex: " << *v << "\n";
								warnings++;
							}
						}
					}
				}
			}
		}
	}

	llvm::outs() << "Warnings: " << warnings << "\n";
}

