#include "PDGUtilities.h"
#include "../graph/algorithms.h"

using namespace llvm;

static long u1_total = 0;
static long u2_total = 0;

size_t calculateSinkReachability(PDG &graph, std::set<Function *> &functions, basegraph::ColorMap &colors) {
	struct FullDataReachability : public basegraph::default_bfs_visitor<PDG> {
		std::set<Function *> &functions;

		explicit FullDataReachability(std::set<Function *> &functions) : functions(functions) {}

		bool examine_forwards_edge(Edge e, const PDG &graph) {
			// return graph[e].type == PDGEdge::Equals;
			return false;
		}

		bool examine_backwards_edge(Edge e, const PDG &graph) {
			// return graph[e].type == PDGEdge::Data || graph[e].type == PDGEdge::Equals || graph[e].type == PDGEdge::Part;
			return graph[e].type == PDGEdge::Data || graph[e].type == PDGEdge::Part;
		}

		bool discover_vertex(Vertex v, PDG &graph) {
			graph[v].reaches_ipc_sink = true;
			auto f = graph[v].function;
			return !f || functions.find(f) != functions.end();
		}
	};
	std::vector<Vertex> sources{};
	//auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
	for (auto f: functions) {
		for (auto v: graph.getVerticesForFunction(f)) {
			if (graph[v].function != f)
				throw std::runtime_error("Error in function vertex index");
			if ((graph[v].ipcsink || graph[v].reaches_ipc_sink))
				sources.push_back(v);
		}
	}
	//auto t2 = std::chrono::high_resolution_clock::now().time_since_epoch();
	breadth_first_search_twoway(graph, FullDataReachability(functions), sources, colors);
	/*
	auto dt2 = std::chrono::high_resolution_clock::now().time_since_epoch() - t2;
	auto dt = t2 - t;
	auto u1 = std::chrono::duration_cast<std::chrono::microseconds>(dt).count();
	auto u2 = std::chrono::duration_cast<std::chrono::microseconds>(dt2).count();
	u1_total += u1;
	u2_total += u2;
	llvm::outs() << "[TIME] " << u1 << "µs    " << u2 << "µs    " << u1_total << "µs    " << u2_total << "µs\n";
	// */
	return sources.size();
}


size_t calculateFDReachability(PDG &graph, std::set<Function *> &functions, basegraph::ColorMap &colors, bool useStructHeuristic) {
	struct ForwardDataReachability : public basegraph::default_bfs_visitor<PDG> {
		std::set<Function *> &functions;
		bool useStructHeuristic;

		explicit ForwardDataReachability(std::set<Function *> &functions, bool useStructHeuristic)
				: functions(functions), useStructHeuristic(useStructHeuristic) {}

		bool examine_forwards_edge(Edge e, const PDG &graph) {
			return graph[e].type == PDGEdge::Data;
		}

		bool examine_backwards_edge(Edge e, const PDG &graph) {
			return false;
		}

		bool discover_vertex(Vertex v, PDG &graph) {
			if (isFileDescriptorType(graph[v].type)) {
				graph[v].filedescriptor = true;
				// TODO UGLY
				if (useStructHeuristic && graph[v].subnode)
					graph.checkIfFDMarkerIsInteresting(v);
				auto f = graph[v].function;
				return !f || functions.find(f) != functions.end();
			}
			return false;
		}
	};
	std::vector<Vertex> sources{};
	if (useStructHeuristic) {
		for (auto v: graph.vertex_set()) {
			// Struct-based heuristics
			if (graph[v].type && graph[v].type->isStructTy()) {
				auto cluster = graph.structClusters.find((const StructType *) graph[v].type);
				if (cluster != graph.structClusters.end()) {
					auto fdParams = graph.structClusterToKnownFDFields.find(cluster->second);
					if (fdParams != graph.structClusterToKnownFDFields.end()) {
						for (auto index: fdParams->second) {
							auto field = graph.getPartOpt(v, index);
							if (field) {
								graph[*field].filedescriptor = true;
							}
						}
					}
				}
			}
		}
	}
	// Default propagation
	for (auto f: functions) {
		for (auto v: graph.getVerticesForFunction(f)) {
			if (graph[v].function != f)
				throw std::runtime_error("Error in function vertex index");
			if (graph[v].filedescriptor)
				sources.push_back(v);
		}
	}
	breadth_first_search_forward(graph, ForwardDataReachability(functions, useStructHeuristic), sources, colors);
	return sources.size();
}

bool isFileDescriptorFunction(llvm::Function *function) {
	if (!function)
		return false;
	auto name = function->getName();
	return name == "open" || name == "openat" || name == "socket";
}
