#include <iostream>
#include "PDGSpecializationPass.h"
#include "PDGCreationPass.h"
#include "PDGReachabilityPass.h"
#include "../graph/algorithms.h"
#include "PDGUtilities.h"
#include "../cali_linker/debug.h"
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/Transforms/Utils/ValueMapper.h>
#include <llvm/IR/ValueMap.h>
#include <llvm/Analysis/CallGraph.h>


using namespace llvm;

char PDGSpecializationPass::ID = 0;

static RegisterPass<PDGSpecializationPass> reg("pdg-specialization", "PDG Specialization and caller -> callee propagation", false, false);


void PDGSpecializationPass::getAnalysisUsage(llvm::AnalysisUsage &Info) const {
	Info.addRequired<PDGCreationPass>();
	Info.addRequired<PDGReachabilityPass>();
	Info.addRequired<DataDrivenSCCPass>();
	Info.addPreserved<PDGCreationPass>();
}

bool PDGSpecializationPass::runOnModule(Module &M) {
	pdg = &getAnalysis<PDGCreationPass>().graph;
	sccpass = &getAnalysis<DataDrivenSCCPass>();
	bool result = false;

	outs() << "--- Specialization pass starts ---" << "\n";

	long count_scc = 0;
	long count_functions = 0;
	long count_removed_old = 0;
	long count_removed_new = 0;

	auto &SCCs = sccpass->getSCCs();
	std::map<Function *, Function *> all_old_to_new;
	for (auto it = SCCs.rbegin(); it != SCCs.rend(); ++it) {
		auto &scc = *it;

		propagateFileDescriptors(scc);

		if (!isSpecializationRequired(scc))
			continue;
		result = true;
		count_scc++;
		count_functions += scc.size();

#ifndef SILENT
		dbg_llvm_outs << "Requires specialization: ";
		for (auto &function: scc) dbg_llvm_outs << " " << (function ? function->getName() : "?");
		dbg_llvm_outs << "\n";
#endif

		// Clone all functions from this SCC
		// update cloned calls to stay in the new SCC
		std::map<Function *, Function *> old_to_new;
		std::set<Function *> new_functions;
		cloneSCC(scc, old_to_new, new_functions);
		// Collect calls that need specialization (including new ones)
		// update outer calls to point to the specialized methods
		// Collect taint from callsides
		for (auto &funcpair: old_to_new) {
			specializeCalls(funcpair.first, funcpair.second);
			all_old_to_new[funcpair.first] = funcpair.second;
		}
		// outs() << "  specialized\n"; //TODO
		// do the dataflow again
		basegraph::ColorMap colors(*pdg);
		calculateSinkReachability(*pdg, new_functions, colors);
		// outs() << "  reachability done.\n"; //TODO
	}

	// Remove functions if not used (anymore?)
	for (auto &funcpair: all_old_to_new) {
		if (removeIfDead(funcpair.second))count_removed_new++;
		else if (removeIfDead(funcpair.first))count_removed_old++;
	}

	outs() << "Specialization finished. " << count_scc << " / " << SCCs.size() << " SCCs have been specialized ("
		   << count_functions << " / " << M.getFunctionList().size() << " functions)\n";
	outs() << "Unused old: " << count_removed_old << "   unused new: " << count_removed_new << "\n";
	outs().flush();
	return result;
}


template<class InstTy>
static inline bool instructionRequiresSpecialization(PDG *pdg, InstTy *ins, Function *function) {
	if (getCalledFunction(ins) != function)
		return false;
	// This is a normal call to a function from our SCC
	// Check for tainted output!
	auto ref = pdg->getDerefOpt(pdg->getVertex(ins));
	if (ref && (*pdg)[*ref].reaches_ipc_sink)
		return true; // "return malloc()"

	std::set<Vertex> visited;
	std::function<bool(Vertex, int)> isShared = [pdg, &isShared, &visited](Vertex v, int limit) {
		if (limit >= 0 && ((*pdg)[v].reaches_ipc_sink || (*pdg)[v].ipcsink))
			return true;
		if (limit > 5 || !visited.insert(v).second)
			return false;
		for (const auto &e: pdg->out_edges(v)) {
			if (e.property.type == PDGEdge::Deref) {
				if (isShared(e.target, limit + 1)) return true;
			} else if (e.property.type == PDGEdge::Part) {
				if (isShared(e.target, limit)) return true;
			}
		}
		return false;
	};

	for (unsigned int i = 0; i < ins->getNumArgOperands(); i++) {
		auto v = pdg->getVertex(ins->getArgOperand(i));
		visited.clear();
		if (isShared(v, -2)) {
			return true; // *arg = malloc()
		}
	}
	return false;
}


bool PDGSpecializationPass::isSpecializationRequired(const std::vector<llvm::Function *> &scc) {
	for (auto function: scc) {
		if (!function || function->isDeclaration())
			continue;
		for (auto &use: function->uses()) {
			//TODO handle bitcasts
			if (isa<CallInst>(use.getUser()) && instructionRequiresSpecialization(pdg, cast<CallInst>(use.getUser()), function)) {
				return true;
			} else if (isa<InvokeInst>(use.getUser()) && instructionRequiresSpecialization(pdg, cast<InvokeInst>(use.getUser()), function)) {
				return true;
			}
		}
	}
	return false;
}

void
PDGSpecializationPass::cloneSCC(const std::vector<llvm::Function *> &scc, std::map<Function *, Function *> &old_to_new, std::set<Function *> &new_functions) {
	// Clone all functions from this SCC, including the PDG structure
	std::map<Vertex, Vertex> vertex_map; // old => new
	for (auto function: scc) {
		ValueToValueMapTy value_map;
		auto newName = getSpecializedName(function->getName());
		auto newFunction = function->getParent()->getFunction(newName);
		if (newFunction) {
			dbg_llvm_outs << "  Found ipc replacement in code: " << function->getName() << "\n";
			old_to_new[function] = newFunction;
			value_map[function] = newFunction;
			new_functions.insert(newFunction);

		} else {

			newFunction = CloneFunction(function, value_map);
			if (!newFunction) {
				llvm::errs() << "Could not clone function " << function->getName() << "\n";
				throw std::exception();
			}
			old_to_new[function] = newFunction;
			newFunction->setName(newName);
			value_map[function] = newFunction;
			new_functions.insert(newFunction);
			// outs() << "  new functions: " << newFunction->getName() << "\n";

			// clone graph things
			cloneGraphForSCCFunction(function, newFunction, value_map, vertex_map);
		}
	}
	cloneGraphForSCCEdges(vertex_map);
	// outs() << "  cloned\n"; //TODO

	// update cloned calls to stay in the new SCC
	// first detect calls, then update. We must not modify use lists while iterating them.
	std::vector<CallInst *> calls;
	std::vector<InvokeInst *> invokes;
	for (auto &it: old_to_new) {
		for (auto &use: it.first->uses()) {
			if (isa<CallInst>(use.getUser())) {
				auto ins = cast<CallInst>(use.getUser());
				if (getCalledFunction(ins) == it.first && new_functions.find(ins->getFunction()) != new_functions.end()) {
					calls.push_back(ins);
				}
			} else if (isa<InvokeInst>(use.getUser())) {
				auto ins = cast<InvokeInst>(use.getUser());
				if (getCalledFunction(ins) == it.first && new_functions.find(ins->getFunction()) != new_functions.end()) {
					invokes.push_back(ins);
				}
			}
		}
	}
	for (auto ins: calls) {
		ins->setCalledFunction(old_to_new[getCalledFunction(ins)]);
	}
	for (auto ins: invokes) {
		ins->setCalledFunction(old_to_new[getCalledFunction(ins)]);
	}
}

void PDGSpecializationPass::cloneGraphForSCCFunction(Function *oldFunction, Function *newFunction, llvm::ValueToValueMapTy &value_map,
													 std::map<Vertex, Vertex> &vertex_map) {
	std::vector<Vertex> sources;
	sources.push_back(pdg->getVertex(oldFunction));
	for (const auto &it: value_map) {
		auto v = pdg->getVertexOpt(it.first);
		if (v) sources.push_back(*v);
	}

	struct ClonedFunctionGen : public basegraph::default_bfs_visitor<PDG> {
		Function *oldFunction;
		Function *newFunction;
		llvm::ValueToValueMapTy &value_map;
		std::map<Vertex, Vertex> &vertex_map;

		ClonedFunctionGen(Function *oldFunction, Function *newFunction, ValueToValueMapTy &valueMap, std::map<Vertex, Vertex> &vertexMap)
				: oldFunction(oldFunction), newFunction(newFunction), value_map(valueMap), vertex_map(vertexMap) {}

		inline bool discover_vertex(Vertex v, PDG &graph) {
			auto ins = graph[v].instruction;
			if (ins && ins == oldFunction)
				return true;
			return graph[v].function == oldFunction;
		}

		inline void examine_vertex(Vertex v, PDG &graph) {
			auto &info = graph[v];
			PDGNode n = info;
			auto value2 = value_map.find(info.instruction);
			if (value2 != value_map.end() && value2->second)
				n.instruction = value2->second;
			if (n.instruction && isa<Function>(n.instruction)) {
				n.function = cast<Function>(n.instruction);
			} else if (info.function) {
				if (info.function == oldFunction) {
					n.function = newFunction;
				} else {
					// that's error debug code
					llvm::errs() << "[] Could not find a matching function for " << info.function->getName() << "\n";
					if (n.instruction && isa<Function>(n.instruction))
						llvm::errs() << "[]   value = function " << cast<Function>(n.instruction)->getName() << "\n";
				}
			}
			auto v2 = graph.pdg_add_vertex(n);
			vertex_map[v] = v2;
			if (value2 != value_map.end() && value2->second)
				graph.value_to_vertex[value2->second] = v2;
		}
	};

	basegraph::ColorMap colors(*pdg);
	breadth_first_search_twoway(*pdg, ClonedFunctionGen(oldFunction, newFunction, value_map, vertex_map), sources, colors);
}


void PDGSpecializationPass::cloneGraphForSCCEdges(std::map<Vertex, Vertex> &vertex_map) {
	for (auto it: vertex_map) {
		for (Edge &e: pdg->out_edges(it.first)) {
			auto it_v2 = vertex_map.find(e.target);
			auto v2 = it_v2 != vertex_map.end() ? it_v2->second : e.target;
			pdg->add_edge_unique(it.second, v2, e.property);
		}
		for (Edge &e: pdg->in_edges(it.first)) {
			if (vertex_map.find(e.source) == vertex_map.end())
				pdg->add_edge_unique(e.source, it.second, e.property);
		}
	}
}


std::string PDGSpecializationPass::getSpecializedName(llvm::StringRef oldName) {
	return std::string("__ipc_specialized_") + oldName.data();
}

template<class InstTy>
static inline void
matchAndPropagateTaint(PDG &graph, Use &use, std::vector<InstTy *> &newCalls, Function *oldFunction, Function *newFunction) {
	auto call = cast<InstTy>(use.getUser());
	if (getCalledFunction(call) != oldFunction)
		return;

	auto vFunc = graph.getVertex(newFunction);
	bool something_reaches_ipc = false;

	std::map<Vertex, Vertex> mapping;
	std::function<void(Vertex, Vertex, int, bool *)> matchSubnodes =
			[&mapping, &graph, &matchSubnodes](Vertex funcParam, Vertex callArg, int derefCounter, bool *something_reaches_ipc) {
				if (mapping.find(funcParam) == mapping.end()) {
					mapping[funcParam] = callArg;
					if (derefCounter >= 0 && (graph[callArg].reaches_ipc_sink || graph[callArg].ipcsink)) {
						graph[funcParam].reaches_ipc_sink = true;
						*something_reaches_ipc = true;
					}
					for (const auto &e: graph.out_edges(funcParam)) {
						if (graph[e].type == PDGEdge::Deref) {
							auto v2 = graph.getDerefOpt(callArg);
							if (v2)
								matchSubnodes(e.target, *v2, derefCounter + 1, something_reaches_ipc);
						} else if (graph[e].type == PDGEdge::Part) {
							auto v2 = graph.getPartOpt(callArg, graph[e].index);
							if (v2)
								matchSubnodes(e.target, *v2, derefCounter, something_reaches_ipc);
						}
					}
				}
			};

	// return value
	auto vFuncRet = graph.getReturnOpt(vFunc);
	if (vFuncRet)
		matchSubnodes(*vFuncRet, graph.getVertex(call), -1, &something_reaches_ipc);

	// params
	for (unsigned int i = 0; i < call->getNumArgOperands(); i++) {
		mapping.clear();
		if (oldFunction->isVarArg() && i >= oldFunction->arg_size()) {
			auto vFuncParam = graph.getParamOpt(vFunc, oldFunction->arg_size());
			if (vFuncParam) {
				matchSubnodes(*vFuncParam, graph.getVertex(call->getArgOperand(i), call->getFunction()), -2, &something_reaches_ipc);
			}
		} else {
			auto vFuncParam = graph.getParamOpt(vFunc, i);
			if (vFuncParam)
				matchSubnodes(*vFuncParam, graph.getVertex(call->getArgOperand(i), call->getFunction()), -2, &something_reaches_ipc);
		}
	}

	if (something_reaches_ipc) {
		newCalls.push_back(call);
		// llvm::outs() << "    Specialize call from " << call->getFunction()->getName() << "\n";
	} else {
		// llvm::outs() << "    Normal call from " << call->getFunction()->getName() << "\n";
	}
}


void PDGSpecializationPass::specializeCalls(Function *oldFunction, Function *newFunction) {
	auto &graph = *pdg;
	// update outer calls to point to the specialized methods
	// Collect taint from callsides

	std::vector<CallInst *> newCalls;
	std::vector<InvokeInst *> newInvokes;

	// follow * and . on both sides, copy over taint, specialize if anything found
	for (auto &use: oldFunction->uses()) {
		if (isa<CallInst>(use.getUser())) {
			/*if (oldFunction->getName() == "retropt_string" && cast<CallInst>(use.getUser())->getFunction()->getName().endswith("_xioopen_openssl_prepare")) {
				llvm::outs() << *use.getUser() << "\n";
				llvm::outs() << "";
			}*/
			matchAndPropagateTaint(graph, use, newCalls, oldFunction, newFunction);
		} else if (isa<InvokeInst>(use.getUser())) {
			/*if (oldFunction->getName() == "_ZN2fz7to_utf8ERKNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEEE" && cast<InvokeInst>(use.getUser())->getFunction()->getName() == "_ZN13CQueueStorageC2Ev") {
				llvm::outs() << *use.getUser() << "\n";
			}*/
			matchAndPropagateTaint(graph, use, newInvokes, oldFunction, newFunction);
		}
	}

	for (auto call: newCalls) {
		call->setCalledFunction(newFunction);
	}
	for (auto call: newInvokes) {
		call->setCalledFunction(newFunction);
	}

	//TODO check uses of that function
#ifndef SILENT
	dbg_llvm_outs << "----- Specialized " << oldFunction->getName() << " -----\n# uses:" << oldFunction->getNumUses() << "  | new uses: "
				 << newFunction->getNumUses() << "\n";
	if (oldFunction->getNumUses() <= 5)
		for (auto &use: oldFunction->uses()) {
			dbg_llvm_outs << "Used by: " << *use.getUser() << "\n";
		}
	dbg_llvm_outs << "----- END " << oldFunction->getName() << " -----\n";
#endif
}

bool PDGSpecializationPass::removeIfDead(llvm::Function *function) {
	if (function->getNumUses() == 0) {
		auto &graph = *pdg;
		graph.removeFunction(function);
		function->eraseFromParent();
		return true;
	} else {
		return false;
	}
}




/**
 * Copy file descriptor markings from a call ("use") to the function signature
 * @tparam InstTy
 * @param graph
 * @param use
 * @param newCalls
 * @param oldFunction
 * @param newFunction
 * @return true if any fd marking has been copied
 */
template<class InstTy>
static inline bool copyFileDescriptors(PDG &graph, InstTy *call, Function *function) {
	auto vFunc = graph.getVertex(function);

	bool has_filedescriptors = false;
	std::map<Vertex, Vertex> mapping;
	std::function<void(Vertex, Vertex, bool *)> matchSubnodes =
			[&mapping, &graph, &matchSubnodes](Vertex funcParam, Vertex callArg, bool *has_filedescriptors) {
				if (mapping.find(funcParam) == mapping.end()) {
					mapping[funcParam] = callArg;
					if (graph[callArg].filedescriptor && isFileDescriptorType(graph[funcParam].type)) {
						graph[funcParam].filedescriptor = true;
						*has_filedescriptors = true;
					}
					for (const auto &e: graph.out_edges(funcParam)) {
						if (graph[e].type == PDGEdge::Deref) {
							auto v2 = graph.getDerefOpt(callArg);
							if (v2)
								matchSubnodes(e.target, *v2, has_filedescriptors);
						} else if (graph[e].type == PDGEdge::Part) {
							auto v2 = graph.getPartOpt(callArg, graph[e].index);
							if (v2)
								matchSubnodes(e.target, *v2, has_filedescriptors);
						}
					}
				}
			};

	// return value
	auto vFuncRet = graph.getReturnOpt(vFunc);
	if (vFuncRet)
		matchSubnodes(*vFuncRet, graph.getVertex(call), &has_filedescriptors);

	// params
	for (unsigned int i = 0; i < call->getNumArgOperands(); i++) {
		mapping.clear();
		auto vFuncParam = graph.getParamOpt(vFunc, i);
		if (vFuncParam)
			matchSubnodes(*vFuncParam, graph.getVertex(call->getArgOperand(i), call->getFunction()), &has_filedescriptors);
	}

	return has_filedescriptors;
}

void PDGSpecializationPass::propagateFileDescriptors(const std::vector<llvm::Function *> &scc) {
	bool update = false;
	for (auto function: scc) {
		if (!function)
			continue;
		for (auto call: getCallsToFunction<CallInst>(function)) {
			if (copyFileDescriptors(*pdg, call, function))
				update = true;
		}
		for (auto call: getCallsToFunction<InvokeInst>(function)) {
			if (copyFileDescriptors(*pdg, call, function))
				update = true;
		}
	}
	if (update) {
		basegraph::ColorMap colors(*pdg);
		std::set<Function*> sccSet(scc.begin(), scc.end());
		calculateFDReachability(*pdg, sccSet, colors, config->strongFDAnalysis);
	}
}
