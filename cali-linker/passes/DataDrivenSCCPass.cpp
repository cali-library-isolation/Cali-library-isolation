#include "DataDrivenSCCPass.h"
#include "../graph/basegraph.h"
#include "../graph/scc_algorithms.h"
#include "../graph/dotwriter.h"
#include <llvm/Analysis/CallGraph.h>
#include <set>
#include <fstream>
#include "../stdlib-is-shit.h"
#include "PDGUtilities.h"

using namespace llvm;


// Data-depdendent call graph (considering only direct calls that introduce data dependency)

namespace {
	struct DataCallGraphEdgeProperty {
	};

	inline bool operator==(const DataCallGraphEdgeProperty &lhs, const DataCallGraphEdgeProperty &rhs) {
		return true;
	}

	class DataCallGraph : public basegraph::BaseGraph<Function *, DataCallGraphEdgeProperty> {
		std::map<Function *, Vertex> function_to_vertex;
	public:
		Vertex getVertex(Function *function) {
			auto it = function_to_vertex.find(function);
			if (it == function_to_vertex.end()) {
				auto v = add_vertex(function);
				function_to_vertex.insert(it, {function, v});
				return v;
			}
			return it->second;
		}
	};

	typedef DataCallGraph::Vertex DGVertex;

	class SCCGraph : public basegraph::BaseGraph<std::vector<DGVertex>, DataCallGraphEdgeProperty> {
	};


	void writeDotForSCC(DataCallGraph &graph, std::vector<llvm::Function *> &scc, const std::string &filename) {
		llvm::outs() << "Exporting (partial) scc graph ..." << "\n";

		struct CGNodeWriter {
			inline void operator()(std::ostream &out, DataCallGraph &graph, DataCallGraph::Vertex v) {
				std::string str;
				llvm::raw_string_ostream rso(str);
				rso << graph[v]->getName();
				rso << "\n" << *graph[v]->getType()->getPointerElementType();
				rso.flush();
				out << " [label=\"";
				out << "#" << v;
				out << ": \\n" << replaceAll(replaceAll(replaceAll(str.substr(0, 200), "\\", "\\\\"), "\"", "\\\""), "\n", "\\n") << "\"";
				out << "]";
			};
		};

		struct CGEdgeWriter {
			inline void operator()(std::ostream &out, const DataCallGraph &graph, const DataCallGraph::Edge &e) {
			};
		};

		struct CGFilter {
			std::set<Function *> &functions;

			explicit CGFilter(std::set<Function *> &functions) : functions(functions) {}

			inline bool operator()(DataCallGraph &graph, DataCallGraph::Vertex v) {
				return functions.find(graph[v]) != functions.end();
			}
		};



		std::ofstream f(filename, std::ios::out | std::ios::binary);
		// basegraph::dot_write_graph(f, graph, PDGNodeWriter(), PDGEdgeWriter(), PDGFunctionFilter());

		std::set<Function *> funcset(scc.begin(), scc.end());
		CGFilter filter(funcset);
		std::vector<DataCallGraph::Vertex> sources;
		for (const auto func: scc) {
			if (func == nullptr)
				continue;
			auto v = graph.getVertex(func);
			sources.push_back(v);
		}
		basegraph::dot_bfs_write_graph(f, graph, sources, CGNodeWriter(), CGEdgeWriter(), filter);

		llvm::outs() << "Wrote " << filename << "\n";
	}
}

// End of data-dependent call graph


static inline int max(int a, long b) {
	return a >= b ? a : (int) b;
}


char DataDrivenSCCPass::ID = 0;
static RegisterPass<DataDrivenSCCPass> reg("data-driven-scc", "SCCs considering only data-transferring calls", false, true);


void DataDrivenSCCPass::getAnalysisUsage(llvm::AnalysisUsage &Info) const {
	Info.setPreservesCFG();
	Info.setPreservesAll();
}

bool DataDrivenSCCPass::runOnModule(llvm::Module &M) {
	outs() << "----- Data-driven SCC generation -----" << "\n";
	dataSCCs.clear();

	// Do my own call graph to split this SCC if possible
	DataCallGraph graph;
	for (auto &function: M.functions()) {
		if (!function.isDeclaration()) {
			DGVertex v = graph.getVertex(&function);
			// find all calls
			for (auto user: function.users()) {
				if (isa<CallInst>(user)) {
					auto callinst = cast<CallInst>(user);
					auto callF = getCalledFunction(callinst);
					if (callF && callF == &function && !callF->isDeclaration()) {
						if (considerCall(callinst)) {
							graph.add_edge(graph.getVertex(callinst->getFunction()), v, {});
						} else {
							// outs() << "Ignore call: " << *callinst << "\n";
						}
					}
				}
				else if (isa<InvokeInst>(user)) {
					auto callinst = cast<InvokeInst>(user);
					auto callF = getCalledFunction(callinst);
					if (callF && callF == &function && !callF->isDeclaration()) {
						if (considerCall(callinst)) {
							graph.add_edge(graph.getVertex(callinst->getFunction()), v, {});
						} else {
							// outs() << "Ignore call: " << *callinst << "\n";
						}
					}
				}
			}
		}
	}

	// Apply Tarjan's algorithm to find SCCs
	SCCGraph sccGraph;
	basegraph::strong_components_graph(graph, sccGraph);
	outs() << "Module with " << graph.num_vertices() << " functions  =>  " << sccGraph.num_vertices() << " new SCCs" << "\n";

	// Store SCCs (callee before caller)
	bottom_up_traversal(sccGraph, [this, &graph](SCCGraph::Vertex v, SCCGraph &sccGraph) {
		std::vector<Function *> functions;
		for (auto v2: sccGraph[v])
			functions.push_back(graph[v2]);
		maxSccSize = max(maxSccSize, functions.size());
		dataSCCs.push_back(std::move(functions));
	});

	//TODO debug
	/*for (auto &scc: dataSCCs) {
		if (scc.size() > 30) {
			std::string fname = "cg-" + std::to_string(scc.size()) + "-" + scc[0]->getName().str() + ".dot";
			writeDotForSCC(graph, scc, fname);
		}
	}*/

	outs() << dataSCCs.size() << " SCCs, maximal size: " << maxSccSize << "\n";
	outs() << "----- Finished data-driven SCC generation -----" << "\n";
	return false;
}


template <class InstTy>
bool DataDrivenSCCPass::considerCall(llvm::CallBase<InstTy> *call) {
	if (!getCalledFunction(call))
		return false;
	auto isNoReturn = call->getNumUses() == 0 || call->getType()->isVoidTy() || call->getType()->isEmptyTy() || call->getType()->isIntegerTy(1) ||
					  call->getType()->isIntegerTy(8) || call->getType()->isFloatingPointTy();
	auto hasNoMeaningfulArgs = true;
	for (unsigned i = 0; i < call->getNumArgOperands(); i++) {
		auto arg = call->getArgOperand(i);
		if (isIgnoredOperand(arg))
			continue;
		// we can loose dataflow not containing enough data for a pointer
		if (arg->getType()->isEmptyTy() || arg->getType()->isIntegerTy(1) || arg->getType()->isIntegerTy(8) || arg->getType()->isFloatingPointTy())
			continue;
		hasNoMeaningfulArgs = false;
	}
	return /*!isNoReturn || */!hasNoMeaningfulArgs;  //TODO optimization, possibly not sound?
}

