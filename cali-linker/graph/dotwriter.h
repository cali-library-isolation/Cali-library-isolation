#ifndef LLVMREWRITER_DOTWRITER_H
#define LLVMREWRITER_DOTWRITER_H

#include <ostream>
#include "algorithms.h"

namespace basegraph {

	template<class Graph, class Vertex>
	struct default_node_filter {
		bool operator()(Graph &graph, Vertex &v) {
			return true;
		}
	};

	template<class Graph, class NodeWriter, class EdgeWriter, class NodeFilter>
	inline void dot_write_graph(std::ostream &out, Graph &graph, NodeWriter nodeWriter, EdgeWriter edgeWriter, NodeFilter filter) {
		out << "digraph pdg {\n";
		for (auto v: graph.vertex_set()) {
			if (filter(graph, v)) {
				out << "v" << v;
				nodeWriter(out, graph, v);
				out << ";\n";

				for (auto &e: graph.out_edges(v)) {
					if (filter(graph, e.target)) {
						out << "v" << v << " -> v" << e.target;
						edgeWriter(out, graph, e);
						out << ";\n";
					}
				}
			}
		}
		out << "}";
	}


	template<class Graph, class Vertex, class NodeWriter, class EdgeWriter, class NodeFilter>
	inline void
	dot_bfs_write_graph(std::ostream &out, Graph &graph, std::vector<Vertex> &sources, NodeWriter nodeWriter, EdgeWriter edgeWriter, NodeFilter &filter,
						const std::string legend = "") {
		out << "digraph pdg {\n";
		out << legend << "\n";

		struct DotPrinter : public default_bfs_visitor<Graph> {
			std::ostream &out;
			NodeWriter &nodeWriter;
			EdgeWriter &edgeWriter;
			NodeFilter &filter;

			DotPrinter(std::ostream &out, NodeWriter &nodeWriter, EdgeWriter &edgeWriter, NodeFilter &filter)
					: out(out), nodeWriter(nodeWriter), edgeWriter(edgeWriter), filter(filter) {}

			inline bool discover_vertex(typename Graph::Vertex v, Graph &graph) {
				if (!filter(graph, v))
					return false;
				out << "v" << v;
				nodeWriter(out, graph, v);
				out << ";\n";
				return true;
			}

			inline bool examine_backwards_edge(const typename Graph::Edge &e, Graph &graph) {
				// if we follow this edge, we will at some point progress the source (and add the edges).
				return filter(graph, e.source);
			}

			inline bool examine_forwards_edge(const typename Graph::Edge &e, Graph &graph) {
				if (!filter(graph, e.target))
					return false;
				out << "v" << e.source << " -> v" << e.target;
				edgeWriter(out, graph, e);
				out << ";\n";
				return true; // true = follow this edge
			}

		};
		ColorMap colors(graph);
		breadth_first_search_twoway(graph, DotPrinter(out, nodeWriter, edgeWriter, filter), sources, colors);

		out << "}";
	}
}

#endif //LLVMREWRITER_DOTWRITER_H
