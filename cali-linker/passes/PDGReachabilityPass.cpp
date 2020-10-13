#include <iostream>
#include <chrono>
#include <boost/graph/breadth_first_search.hpp>
#include "PDGReachabilityPass.h"
#include "PDGCreationPass.h"
#include "../graph/algorithms.h"
#include "../graph/color_map.h"
#include "PDGUtilities.h"
#include "../cali_linker/debug.h"
#include <llvm/IR/Constants.h>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/Support/Format.h>

using namespace llvm;
using namespace boost;


char PDGReachabilityPass::ID = 0;

static RegisterPass<PDGReachabilityPass> reg("pdg-reachability", "PDG data flow calculation", false, true);

void PDGReachabilityPass::getAnalysisUsage(AnalysisUsage &Info) const {
	Info.setPreservesCFG();
	Info.setPreservesAll();
	Info.addRequired<PDGCreationPass>();
	Info.addRequired<DataDrivenSCCPass>();
}

static long function_count = 0;

bool PDGReachabilityPass::runOnModule(Module &M) {
	pdg = &getAnalysis<PDGCreationPass>().graph;
	sccpass = &getAnalysis<DataDrivenSCCPass>();

	runOnGlobal();
	for (auto &scc: sccpass->getSCCs()) {
		runOnSCC(scc);
	}
	if (config->strongFDAnalysis) {
		for (auto &scc: sccpass->getSCCs()) {
			std::set<Function *> functions(scc.begin(), scc.end());
			calculateFDReachability(*pdg, functions, *colors, config->strongFDAnalysis);
		}
	}
	runOnGlobal();

	return false;
}


void PDGReachabilityPass::runOnSCC(const std::vector<llvm::Function *> &scc) {
	auto &graph = *pdg;
	function_count += scc.size();
	dbg_llvm_outs << "SCC with " << format("%2ld", scc.size()) << " functions (" << format("%4d", function_count) << " total)";

	/*
	outs() << "SCC: " << scc.size() << "\n";
	for (auto &x: scc) {
		outs() << "- " << (x->getFunction() ? x->getFunction()->getName() : "(indirect)");
		outs() << "\n";
	}
	 */
	/*if (function_count > 4000) exit(0);  // TODO debug
	outs() << "   v=" << graph.num_vertices() << " e=" << format("%8ld", graph.num_edges());
	long count_data = 0;
	long count_summary = 0;
	long count_structural = 0;
	long count_other = 0;
	for (auto &v: graph.vertices) {
		for (auto &e: v.out_edges) {
			switch (e.property.type) {
				case PDGEdge::PDGEdgeType::Data:
					count_data++;
					break;
				case PDGEdge::PDGEdgeType::Summary_Data:
					count_summary++;
					break;
				case PDGEdge::PDGEdgeType::Deref:
				case PDGEdge::PDGEdgeType::Part:
					count_structural++;
					break;
				default:
					count_other++;
			}
		}
	}
	outs() << "  edges=(" << format("%7ld, %8ld, %7ld, %7ld", count_data, count_summary, count_structural, count_other) << ")";
	 */

	std::set<Function *> functions(scc.begin(), scc.end());
	dbg_llvm_outs << "    ";
	for (auto f: scc) dbg_llvm_outs << f->getName() << "  ";
	// outs() << "\n";

	// Instantiate call instructions (summary / sharing)
	std::vector<Vertex> varargNodes;
	std::vector<CallInst *> indirectCalls;
	std::vector<InvokeInst *> indirectInvokes;

	for (auto function: scc) {
		for (auto &bb: *function) {
			for (auto &ins: bb) {
				if (isa<CallInst>(ins)) {
					handleCall(indirectCalls, function, ins, functions);
				} else if (isa<InvokeInst>(ins)) {
					handleCall(indirectInvokes, function, ins, functions);
				}
			}
		}
		if (function->isVarArg()) {
			Vertex vf = graph.getVertex(function);
			PDGNode n;
			n.function = function;
			n.type = Type::getVoidTy(function->getContext());
			n.subnode = true;
			auto newNode = graph.pdg_add_vertex(n);
			graph.add_edge(vf, newNode, PDGEdge{.type= PDGEdge::Param, .index=(unsigned int) function->arg_size()});
			varargNodes.push_back(newNode);
		}
	}

	colors = std::make_shared<basegraph::ColorMap>(graph);

	calculateIpcFunctionCalls(indirectCalls);
	calculateIpcFunctionCalls(indirectInvokes);

	// Calculate reachability (sinks, sources, params, return)
	if (calculateSinkReachability(*pdg, functions, *colors) > 0) {
		for (Vertex v: varargNodes) {
			graph[v].reaches_ipc_sink = true;
		}
	}
	calculateFDReachability(*pdg, functions, *colors, config->strongFDAnalysis);

	// auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
	calculateSummaryEdges(functions);
	// auto dt = std::chrono::high_resolution_clock::now().time_since_epoch() - t;
	// std::cout << "dt = " << std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() << " ms" << std::endl;

	// TODO Summarize sources -> {params, return}

	dbg_llvm_outs << "\n";
}

void PDGReachabilityPass::runOnGlobal() {
	auto &graph = *pdg;
	std::set<Function *> functions({nullptr});
	colors = std::make_shared<basegraph::ColorMap>(graph);
	calculateSinkReachability(*pdg, functions, *colors);
	calculateFDReachability(*pdg, functions, *colors, config->strongFDAnalysis);
}


template<class InstTy>
void PDGReachabilityPass::handleCall(std::vector<InstTy *> &indirectCalls, Function *function, Instruction &ins, std::set<Function *> &functions) {
	auto &call = cast<InstTy>(ins);

	auto handleCallTarget = [this, function, &ins, functions, &call](Function *calledFunction) {
		auto &graph = *pdg;
		if (calledFunction && addKnownFunctionSummary(calledFunction, call)) {
			// function has been already handled (memcpy etc)
		} else if (calledFunction && functions.find(calledFunction) != functions.end()) {
			// Function is in the same SCC - add direct edges
			auto vCall = graph.getVertex(&ins);
			auto vFunc = graph.getVertex(calledFunction);
			// params
			for (unsigned int i = 0; i < call.getNumArgOperands(); i++) {
				auto vFuncParam = graph.getParamOpt(vFunc, i);
				if (vFuncParam && !isIgnoredOperand(call.getArgOperand(i)))
					// We do not want data flow coming out of a parameter - therefore reduced unification
					graph.addSubnodeEdges(graph.getVertex(call.getArgOperand(i), function), *vFuncParam, 1);
			}
			// return value
			auto vFuncRet = graph.getReturnOpt(vFunc);
			if (vFuncRet)
				graph.addSubnodeEdges(*vFuncRet, vCall);

		} else if (calledFunction && isa<Function>(calledFunction)) {
			// Function is in another SCC - there must be summary edges
			auto vCall = graph.getVertex(&ins);
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
						if (graph[*vFuncParam].reaches_ipc_sink) {
							markSubnodesReachingIpc(graph.getVertex(call.getArgOperand(i), function));
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

			// copy summary edges and ipcsink property
			for (const auto &it: matching) {
				if (graph[it.first].reaches_ipc_sink || graph[it.first].ipcsink) {
					graph[it.second].reaches_ipc_sink = true;
				}
				if (graph[it.first].filedescriptor && isFileDescriptorType(graph[it.second].type)) {
					graph[it.second].filedescriptor = true;
				}
				for (const auto &e: graph.out_edges(it.first)) {
					if (graph[e].type == PDGEdge::Summary_Equals || graph[e].type == PDGEdge::Summary_Data) {
						const auto &callsideDest = matching.find(e.target);
						if (callsideDest != matching.end()) {
							graph.add_edge(it.second, callsideDest->second,
										   PDGEdge{.type=(graph[e].type == PDGEdge::Summary_Equals ? PDGEdge::Equals : PDGEdge::Data), .index=0});
						}
					}
					// Invocations of function pointer parameters?
					if (graph[e].type == PDGEdge::Invocation || graph[e].type == PDGEdge::Summary_Invocation) {
						// add dummy call node, link to current call
						auto dummyCallNode = graph.createDummyVertex(&call, call.getFunction());
						graph.add_edge(it.second, dummyCallNode, PDGEdge{.type=PDGEdge::Invocation, .index=0});
						// copy all parameter edges
						for (const auto &e2: graph.in_edges(e.target)) {
							if (graph[e2].type == PDGEdge::Param) {
								auto it2 = matching.find(e2.source);
								if (it2 != matching.end()) {
									// copy/summarize Param edge
									graph.add_edge(it2->second, dummyCallNode, PDGEdge{.type=PDGEdge::Param, .index=graph[e2].index});
								}
							}
						}
					}
				}
			}
		}

		// Check if function needs to be marked
		if (calledFunction) {
			if (graph[graph.getVertex(calledFunction)].ipcfunction)
				markIpcCall(&call);
		}
	};

	auto calledFunction = getCalledFunction(call);
	handleCallTarget(calledFunction);
	if (!calledFunction) {
		// Indirect call
		if (config->strongFPAnalysis) {
			auto &graph = *pdg;
			for (auto calledFunction: *sccpass->getPossibleIndirectFunctions(&call)) {
				if (!calledFunction->hasName() || graph[graph.getVertex(calledFunction)].function == nullptr) {
					llvm::errs() << "[WARN] invalid function: " << calledFunction->isDeclaration() << " | " << *calledFunction << "\n";
				} else {
					handleCallTarget(calledFunction);
				}
			}
		}
		indirectCalls.push_back(&call);
	}
}


template<class InstTy>
bool PDGReachabilityPass::addKnownFunctionSummary(llvm::Function *calledFunction, InstTy &call) {
	if (calledFunction->getName().startswith("llvm.memcpy.")) {
		auto srcMemory = pdg->getDerefOpt(pdg->getVertex(call.getArgOperand(1)));
		auto dstMemory = pdg->getDerefOpt(pdg->getVertex(call.getArgOperand(0)));
		if (srcMemory && dstMemory)
			pdg->addSubnodeEdges(*srcMemory, *dstMemory);
		// outs() << "    Handling memcpy: " << call << "\n";
		return true;
	}
	if (calledFunction->getName() == "realloc") {
		auto srcMemory = pdg->getDerefOpt(pdg->getVertex(call.getArgOperand(0)));
		auto dstMemory = pdg->getDerefOpt(pdg->getVertex(&call));
		if (srcMemory && dstMemory)
			pdg->addSubnodeEdges(*srcMemory, *dstMemory);
		return true;
	}
	//*
	if (calledFunction->getName().endswith("basic_stringIcSt11char_traitsIcESaIcEE5c_strEv")) {
		auto resultMemory = pdg->getDerefOpt(pdg->getVertex(&call));
		if (!resultMemory) return false;
		auto string = pdg->getDerefOpt(pdg->getVertex(call.getArgOperand(0)));
		if (!string) return false;
		auto some_union = pdg->getPartOpt(*string, 2);
		if (some_union) {
			auto data_from_union = pdg->getPartOpt(*some_union, 1);
			if (data_from_union) {
				pdg->uniteSubnodes(*resultMemory, *data_from_union);
			}
		}
		int counter = 5;
		while (string) {
			auto mem = pdg->getDerefOpt(*string);
			if (mem)
				pdg->uniteSubnodes(*resultMemory, *mem);
			string = pdg->getPartOpt(*string, 0);
			if (counter-- == 0) break;
		}
		return false;
	}
	//*/
	return false;
}


template<class InstTy>
void PDGReachabilityPass::calculateIpcFunctionCalls(std::vector<InstTy *> indirectCalls) {
	auto &graph = *pdg;

	struct IpcFunctionReachability : public basegraph::default_bfs_visitor<PDG> {
		/*std::set<Function *> &functions;

		explicit FullDataReachability(std::set<Function *> &functions) : functions(functions) {}*/

		inline bool examine_forwards_edge(Edge e, const PDG &graph) {
			return graph[e].type == PDGEdge::Deref; // Follow function pointers to actual function
		}

		inline bool examine_backwards_edge(Edge e, const PDG &graph) {
			return graph[e].type == PDGEdge::Data || graph[e].type == PDGEdge::Part;
		}

		inline bool is_wanted_vertex(Vertex v, PDG &graph) {
			return graph[v].ipcfunction;
		}
	};

	for (auto call: indirectCalls) {
		// outs() << "    Indirect called value: " << *call->getCalledValue() << "\n";
		auto source = graph.getVertex(call->getCalledValue());
		if (breadth_first_search_twoway(graph, IpcFunctionReachability(), source, *colors)) {
			markIpcCall(call);
			dbg_llvm_outs << "    -> Found indirect IPC call: " << *call << "\n";
		}
	}
}



void PDGReachabilityPass::calculateSummaryEdges(std::set<Function *> &functions) {
	auto &graph = *pdg;
	std::set<Vertex> nodes;
	for (auto f: functions) {
		// Check if function summary is ever needed by something outside of this SCC
		for (auto user: f->users()) {
			if (isa<CallInst>(user)) {
				auto ins = cast<CallInst>(user);
				if (getCalledFunction(ins) == f && DataDrivenSCCPass::considerCall(ins)) {
					if (functions.find(ins->getFunction()) == functions.end()) {
						// enqueue function signature for summary analysis
						collectFunctionSignatureNodes(f, nodes);
						break;
					}
				}
			} else if (isa<InvokeInst>(user)) {
				auto ins = cast<InvokeInst>(user);
				if (getCalledFunction(ins) == f && DataDrivenSCCPass::considerCall(ins)) {
					if (functions.find(ins->getFunction()) == functions.end()) {
						// enqueue function signature for summary analysis
						collectFunctionSignatureNodes(f, nodes);
						break;
					}
				}
			}
		}
	}

	struct DataSummaryVisitor : public basegraph::default_bfs_visitor<PDG> {
		Vertex start;
		std::set<Vertex> &nodes;
		std::set<Function *> &functions;
		std::vector<Vertex> new_edges;
		long &edge_count;

		DataSummaryVisitor(Vertex start, std::set<Vertex> &nodes, std::set<Function *> &functions, long &edge_count)
				: start(start), nodes(nodes), functions(functions), edge_count(edge_count) {}

		inline bool examine_forwards_edge(Edge e, const PDG &graph) {
			// return graph[e].type == PDGEdge::Equals || graph[e].type == PDGEdge::Data;
			return graph[e].type == PDGEdge::Data || graph[e].type == PDGEdge::Summary_Data || graph[e].type == PDGEdge::Part;
		}

		inline bool examine_backwards_edge(Edge e, const PDG &graph) {
			// return graph[e].type == PDGEdge::Equals;
			return false;
		}

		inline bool discover_vertex(Vertex v, PDG &graph) {
			if (v != start) {
				if (nodes.find(v) != nodes.end()) {
					// graph.add_edge(start, v, PDGEdge{.type=PDGEdge::Summary_Data});
					new_edges.push_back(v);
				} else {
					auto f = graph[v].function;
					if (f && functions.find(f) == functions.end())
						return false;
				}
			}
			return true;
		}

		void finished(PDG &graph) {
			edge_count += new_edges.size();
			auto derefStart = graph.getDerefOpt(start);
			for (auto &target: new_edges) {
				graph.add_edge(start, target, PDGEdge{.type=PDGEdge::Summary_Data, .index=0});
				// Workaround - if a data-flows to b and *b is shared, then *a must also be shared.
				if (derefStart && !graph[*derefStart].reaches_ipc_sink) {
					auto derefTarget = graph.getDerefOpt(target);
					if (derefTarget && (graph[*derefTarget].reaches_ipc_sink || graph[*derefTarget].ipcsink)) {
						markAsShared(graph, *derefStart);
					}
				}
			}
		}
	};

	struct InvocationSummaryVisitor : public basegraph::default_bfs_visitor<PDG> {
		Vertex start;
		std::set<Vertex> &nodes;
		std::set<Function *> &functions;
		std::vector<Vertex> new_edges_invoke;
		long &edge_count;

		InvocationSummaryVisitor(Vertex start, std::set<Vertex> &nodes, std::set<Function *> &functions, long &edge_count)
				: start(start), nodes(nodes), functions(functions), edge_count(edge_count) {}

		inline bool examine_forwards_edge(Edge e, const PDG &graph) {
			return graph[e].type == PDGEdge::Data; // || graph[e].type == PDGEdge::Summary_Data;
		}

		inline bool examine_backwards_edge(Edge e, const PDG &graph) {
			return false;
		}

		inline bool discover_vertex(Vertex v, PDG &graph) {
			if (v != start) {
				if (nodes.find(v) != nodes.end()) {
					// Summary edge: From summary node to any other call instruction (not necessarily summary node)
					for (const auto &e: graph.out_edges(v)) {
						if (graph[e].type == PDGEdge::Invocation) {
							new_edges_invoke.push_back(e.target);
						}
					}
				} else {
					// search only within SCC
					auto f = graph[v].function;
					if (f && functions.find(f) == functions.end())
						return false;
				}
			}
			return true;
		}

		void finished(PDG &graph) {
			edge_count += new_edges_invoke.size();
			for (auto &target: new_edges_invoke)
				graph.add_edge(start, target, PDGEdge{.type=PDGEdge::Summary_Invocation, .index=0});
		}
	};

	long count = 0;
	long edge_count = 0;
	for (auto v: nodes) {
		// actually: forward search, taking backward "=" edges into account
		breadth_first_search_forward(graph, DataSummaryVisitor(v, nodes, functions, edge_count), v, *colors);
		if (config->strongFPAnalysis) {
			if (graph[v].type->isPointerTy() || graph[v].type->isFunctionTy())
				breadth_first_search_forward(graph, InvocationSummaryVisitor(v, nodes, functions, edge_count), v, *colors);
		}
#ifndef SILENT
		if (nodes.size() > 10000) {
			// break; //TODO hack
			count++;
			if (count % 100 == 0)
				dbg_llvm_outs << "\n    calculateSummaryEdges " << count << " / " << nodes.size();
		}
#endif
	}
	dbg_llvm_outs << "  summary=" << edge_count << " between " << nodes.size() << " nodes";
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuninitialized"

void PDGReachabilityPass::collectFunctionSignatureNodes(Function *f, std::set<Vertex> &nodes) {
	auto &graph = *pdg;
	for (const auto &e: graph.out_edges(graph.getVertex(f))) {
		if (graph[e].type == PDGEdge::Return || graph[e].type == PDGEdge::Param)
			collectSubnodesForFunction(e.target, nodes, f);
	}
}

void PDGReachabilityPass::collectSubnodesForFunction(Vertex v, std::set<Vertex> &nodes, Function *f) {
	auto &graph = *pdg;
	if (nodes.insert(v).second) {
		for (const auto &e: graph.out_edges(v)) {
			if ((graph[e].type == PDGEdge::Deref || graph[e].type == PDGEdge::Part) && graph[e.target].function == f)
				collectSubnodesForFunction(e.target, nodes, f);
		}
	}
}

void PDGReachabilityPass::matchSubnodes(std::map<Vertex, Vertex> &map, Vertex key, Vertex value) {
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

void PDGReachabilityPass::markSubnodesReachingIpc(Vertex root) {
	auto &graph = *pdg;
	if (!graph[root].reaches_ipc_sink) {
		graph[root].reaches_ipc_sink = true;
		for (const auto &e: graph.out_edges(root)) {
			if (graph[e].type == PDGEdge::Deref || graph[e].type == PDGEdge::Part) {
				markSubnodesReachingIpc(e.target);
			}
		}
	}
}

#pragma clang diagnostic pop

template<class InstTy>
void PDGReachabilityPass::markIpcCall(InstTy *call) {
	auto v = pdg->getVertex(call);

	for (auto &arg: call->arg_operands()) {
		// tmp for readability
		if (isIgnoredOperand(arg.get()))
			continue;
		auto v2 = pdg->getVertex(arg.get());
		pdg->add_edge(v2, v, PDGEdge{.type = PDGEdge::Param, .index=arg.getOperandNo()});
		markAsPointerToShared(*pdg, v2);
	}
}
