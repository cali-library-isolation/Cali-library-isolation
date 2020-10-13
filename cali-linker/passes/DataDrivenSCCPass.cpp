#include "DataDrivenSCCPass.h"
#include "../graph/basegraph.h"
#include "../graph/scc_algorithms.h"
#include "../graph/dotwriter.h"
#include <llvm/Analysis/CallGraph.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Value.h>
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


	void writeDotForSCC(DataCallGraph &graph, const std::vector<llvm::Function *> &scc, const std::string &filename) {
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

	if (config->strongFDAnalysis || config->strongFPAnalysis) {
		analyzeStructs(M);
	}

	// Do my own call graph to split this SCC if possible
	DataCallGraph graph;
	for (auto &function: M.functions()) {
		if (!function.isDeclaration() && considerFunction(&function)) {
			DGVertex v = graph.getVertex(&function);
			// find all calls
			for (auto user: function.users()) {
				if (isa<CallInst>(user)) {
					auto callinst = cast<CallInst>(user);
					auto callF = getCalledFunction(callinst);
					if (callF && callF == &function && !callF->isDeclaration()) {
						if (considerCall(callinst) && considerFunction(callinst->getFunction())) {
							graph.add_edge(graph.getVertex(callinst->getFunction()), v, {});
						} else {
							// outs() << "Ignore call: " << *callinst << "\n";
						}
					}
				} else if (isa<InvokeInst>(user)) {
					auto callinst = cast<InvokeInst>(user);
					auto callF = getCalledFunction(callinst);
					if (callF && callF == &function && !callF->isDeclaration()) {
						if (considerCall(callinst) && considerFunction(callinst->getFunction())) {
							graph.add_edge(graph.getVertex(callinst->getFunction()), v, {});
						} else {
							// outs() << "Ignore call: " << *callinst << "\n";
						}
					}
				}
			}
		}
		if (config->strongFPAnalysis) {
			for (auto user: function.users()) {
				if (isa<CallInst>(user)) continue;
				if (isa<InvokeInst>(user)) continue;
				if (isa<GlobalVariable>(user)) continue;
				if (isa<ICmpInst>(user)) continue;
				// outs() << "Indirect function usage: " << *user << "\n";
				possibleIndirectCallTargets.push_back(&function);
				break;
			}
		}
	}

	llvm::outs() << "Indirect functions: " << possibleIndirectCallTargets.size() << " / " << M.getFunctionList().size() << "\n";

	// Find indirect function targets
	for (auto &f: M.functions()) {
		if (!considerFunction(&f)) continue;
		for (auto &bb: f) {
			for (auto &ins: bb) {
				if (isa<CallInst>(ins)) {
					auto callinst = cast<CallInst>(&ins);
					auto callF = getCalledFunction(callinst);
					if (!callF && considerCall(callinst, false)) {
						for (auto f2: *getPossibleIndirectFunctions(callinst)) {
							if (!f2->isDeclaration()) graph.add_edge(graph.getVertex(&f), graph.getVertex(f2), {});
						}
					}
				} else if (isa<InvokeInst>(ins)) {
					auto callinst = cast<InvokeInst>(&ins);
					auto callF = getCalledFunction(callinst);
					if (!callF && considerCall(callinst, false)) {
						for (auto f2: *getPossibleIndirectFunctions(callinst)) {
							if (!f2->isDeclaration()) graph.add_edge(graph.getVertex(&f), graph.getVertex(f2), {});
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
	/* for (auto &scc: dataSCCs) {
		if (scc.size() > 30) {
			std::string fname = "cg-" + std::to_string(scc.size()) + "-" + scc[0]->getName().str() + ".dot";
			writeDotForSCC(graph, scc, fname);
		}
	} //*/
	// writeDotForSCC(graph, {M.getFunction("decompress_filter"), M.getFunction("decompress_gzip")}, "cg-test.dot");

	outs() << dataSCCs.size() << " SCCs, maximal size: " << maxSccSize << "\n";
	outs() << "----- Finished data-driven SCC generation -----" << "\n";
	return false;
}


template<class InstTy>
bool DataDrivenSCCPass::considerCall(llvm::CallBase<InstTy> *call, bool needsFunction) {
	if (needsFunction && !getCalledFunction(call))
		return false;
	/*auto isNoReturn = call->getNumUses() == 0 || call->getType()->isVoidTy() || call->getType()->isEmptyTy() || call->getType()->isIntegerTy(1) ||
					  call->getType()->isIntegerTy(8) || call->getType()->isFloatingPointTy();*/
	auto hasReturn = call->getNumUses() > 0 && (call->getType()->isStructTy() || call->getType()->isPointerTy());
	auto hasMeaningfulArgs = false;
	for (unsigned i = 0; i < call->getNumArgOperands(); i++) {
		auto arg = call->getArgOperand(i);
		if (isIgnoredOperand(arg))
			continue;
		// we can loose dataflow not containing enough data for a pointer
		if (arg->getType()->isEmptyTy() || arg->getType()->isIntegerTy(1) || arg->getType()->isIntegerTy(8) || arg->getType()->isFloatingPointTy())
			continue;
		hasMeaningfulArgs = true;
		break;
	}
	return hasReturn || hasMeaningfulArgs;
}


template<class InstTy>
std::shared_ptr<const std::vector<Function *>> DataDrivenSCCPass::getPossibleIndirectFunctions(llvm::CallBase<InstTy> *call) {
	auto it = possibleInstructionTargets.find(call);
	if (it != possibleInstructionTargets.end())
		return it->second;
	auto possibleTargets = std::make_shared<std::vector<Function *>>();
	possibleInstructionTargets[call] = possibleTargets;
	// llvm::outs() << "Determining for " << *call << "\n";

	Type *type = call->getCalledValue()->getType();
	FunctionType *ft = dyn_cast<FunctionType>(type);
	if (!ft && type->isPointerTy())
		ft = dyn_cast<FunctionType>(type->getPointerElementType());

	if (ft) {
		for (auto f: possibleIndirectCallTargets) {
			if (isCompatibleCallType(ft, f->getFunctionType())) {
				possibleTargets->push_back(f);
				auto v = (std::vector<Instruction *>*) getPossibleIndirectCalls(f).get();
				v->push_back(call);
			}
		}
	}

	/*for (auto f: possibleTargets) {
		llvm::outs() << " - " << f->getName() << "\n";
	}*/
	return possibleTargets;
}

template std::shared_ptr<const std::vector<Function *>> DataDrivenSCCPass::getPossibleIndirectFunctions(llvm::CallBase<CallInst> *call);

template std::shared_ptr<const std::vector<Function *>> DataDrivenSCCPass::getPossibleIndirectFunctions(llvm::CallBase<InvokeInst> *call);

std::shared_ptr<const std::vector<llvm::Instruction*>> DataDrivenSCCPass::getPossibleIndirectCalls(llvm::Function* function) {
	auto it = possibleFunctionCalls.find(function);
	if (it != possibleFunctionCalls.end())
		return it->second;
	auto possibleTargets = std::make_shared<std::vector<Instruction *>>();
	possibleFunctionCalls[function] = possibleTargets;
	return possibleTargets;
}



bool DataDrivenSCCPass::isCompatibleCallType(llvm::FunctionType *callType, llvm::FunctionType *functionType) {
	if (!callType->isVarArg() && functionType->getNumParams() > callType->getNumParams()) return false;
	if (!functionType->isVarArg() && callType->getNumParams() > functionType->getNumParams()) return false;

	const std::function<bool(const Type *, const Type *)> isCompatible = [this, &isCompatible](const Type *t1, const Type *t2) {
		const auto isVoidPtr = [](const Type *t) {
			return t->isPointerTy() && t->getPointerElementType()->isIntegerTy() && t->getPointerElementType()->getIntegerBitWidth() == 8;
		};
		const auto isVoidPtr2 = [](const Type *t) {
			return t->isPointerTy() && t->getPointerElementType()->isEmptyTy() && t->getPointerElementType()->isStructTy();
		};

		if (t1->isVoidTy()) return t2->isVoidTy();
		if (t1->isIntegerTy()) return t2->isIntegerTy() && t1->getIntegerBitWidth() == t2->getIntegerBitWidth();
		if (t1->isFloatTy() || t1->isDoubleTy()) return t2->isFloatTy() || t2->isDoubleTy();
		if (t1->isFunctionTy()) return t2->isFunctionTy();
		if (t1->isPointerTy()) {
			if (t2->isPointerTy()) {
				// void* = i8*
				return // isVoidPtr(t1) || isVoidPtr(t2) ||
						isVoidPtr2(t1) || isVoidPtr2(t2) ||
						isCompatible(t1->getPointerElementType(), t2->getPointerElementType());
			}
			return false;
		}
		if (t1->isArrayTy()) return t2->isArrayTy() && isCompatible(t1->getArrayElementType(), t2->getArrayElementType());
		if (t1->isStructTy()) return t2->isStructTy() && structClusters.find(cast<StructType>(t1))->second == structClusters.find(cast<StructType>(t2))->second;
		llvm::outs() << "[WARN] Unhandled type: " << *t1 << " =?= " << *t2 << "\n";
		return true;
	};

	if (!isCompatible(callType->getReturnType(), functionType->getReturnType()))
		return false;
	for (int i = 0; i < callType->getNumParams() && i < functionType->getNumParams(); i++) {
		if (!isCompatible(callType->getParamType(i), functionType->getParamType(i)))
			return false;
	}
	return true;
}


static bool struct_deep_equal(const StructType *st1, const StructType *st2, std::map<const StructType *, int> &equality) {
	if (st1->getStructNumElements() != st2->getStructNumElements())
		return false;
	for (unsigned int i = 0; i < st1->getStructNumElements(); i++) {
		auto t1 = st1->getStructElementType(i);
		auto t2 = st2->getStructElementType(i);
		if (t1 == t2)
			continue;
		if (t1->isPointerTy() && t2->isPointerTy()) {
			// void*
			if (t1->getPointerElementType()->isEmptyTy() || t2->getPointerElementType()->isEmptyTy())
				continue;
			// pointer to struct
			if (t1->getPointerElementType()->isStructTy() && t2->getPointerElementType()->isStructTy()) {
				auto eq1 = equality.find((StructType *) t1->getPointerElementType());
				auto eq2 = equality.find((StructType *) t2->getPointerElementType());
				if (eq1 != equality.end() && eq2 != equality.end() && eq1->second == eq2->second)
					continue;
			}
		}
		if (t1->isStructTy() && t2->isStructTy()) {
			auto eq1 = equality.find((StructType *) t1);
			auto eq2 = equality.find((StructType *) t2);
			if (eq1 != equality.end() && eq2 != equality.end() && eq1->second == eq2->second)
				continue;
		}
		return false;
	}
	return true;
}

void DataDrivenSCCPass::analyzeStructs(llvm::Module &M) {
	std::map<std::string, int> nameToCluster;
	std::vector<std::shared_ptr<std::set<const StructType *>>> clusters;
	for (const auto st: M.getIdentifiedStructTypes()) {
		auto str = st->getStructName();
		str = str.substr(0, str.find_last_not_of(".0123456789") + 1);
		auto it = nameToCluster.find(str);
		if (it != nameToCluster.end()) {
			structClusters[st] = it->second;
			clusters[it->second]->insert(st);
		} else {
			clusters.emplace_back(new std::set<const StructType *>());
			clusters.back()->insert(st);
			structClusters[st] = clusters.size() - 1;
			nameToCluster[str] = clusters.size() - 1;
		}
	}

	// Post-filter struct equality
	for (size_t i = 0; i < clusters.size(); i++) {
		size_t new_cluster_index = 0;
		const StructType *ref = nullptr;
		for (const auto st: *clusters[i]) {
			if (ref) {
				if (!struct_deep_equal(st, ref, structClusters)) {
					if (!new_cluster_index) {
						clusters.emplace_back(new std::set<const StructType *>());
						new_cluster_index = clusters.size() - 1;
					}
					clusters[new_cluster_index]->insert(st);
				}
			} else {
				ref = st;
			}
		}
		if (new_cluster_index) {
			for (const auto st: *clusters[new_cluster_index]) {
				structClusters[st] = new_cluster_index;
			}
		}
	}
}

bool DataDrivenSCCPass::considerFunction(llvm::Function *function) {
	auto &ignored = contextConfig->ignoredFunctions;
	if (std::find(ignored.begin(), ignored.end(), function->getName()) != ignored.end())
		return false;
	if (contextConfig->functionBehavior.find(function->getName()) != contextConfig->functionBehavior.end())
		return false;
	return true;
}
