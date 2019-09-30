#ifndef LLVMREWRITER_ALGORITHMS_H
#define LLVMREWRITER_ALGORITHMS_H

#include "basegraph.h"
#include "color_map.h"
#include "abstract_map.h"
#include "../cali_linker/timing-utils.h"
#include <boost/pending/queue.hpp>
#include <iostream>

namespace basegraph {

	template<class Graph>
	struct default_bfs_visitor {
		inline bool discover_vertex(typename Graph::Vertex, Graph &) {
			return true; // true = consider this vertex
		}

		inline bool is_wanted_vertex(typename Graph::Vertex, Graph &) {
			return false;
		}

		/**
		 * You can modify everything here, as long as you don't delete this vertex and vertices in the "gray" state
		 */
		inline void examine_vertex(typename Graph::Vertex, Graph &) {}

		/**
		 * You can modify everything here, as long as you don't delete vertices in the "gray" state
		 */
		inline void finish_vertex(typename Graph::Vertex, Graph &) {}

		inline bool examine_backwards_edge(const typename Graph::Edge &, Graph &) {
			return true; // true = follow this edge
		}

		inline bool examine_forwards_edge(const typename Graph::Edge &, Graph &) {
			return true; // true = follow this edge
		}

		inline void tree_edge(const typename Graph::Edge &, Graph &) {}

		inline void finished(Graph &) {}

	};

	template<class Graph, class BFSVisitor>
	bool breadth_first_search_twoway(Graph &graph, BFSVisitor visitor, std::vector<typename Graph::Vertex> sources, ColorMap &colors) {
		if (sources.empty())
			return false;
		// Init
		boost::queue<typename Graph::Vertex> Q;
		colors.init();
		for (const auto s: sources) {
			colors.put_fast(s, GRAY);
			if (visitor.is_wanted_vertex(s, graph))
				return true;
			if (visitor.discover_vertex(s, graph))
				Q.push(s);
		}


		// Traverse graph
		while (!Q.empty()) {
			auto u = Q.top();
			Q.pop();

			visitor.examine_vertex(u, graph);
			// std::cout << "DEBUG: " << u << " (" << colors.get(u) << ") size = " << Q.size() << std::endl;

			// backward edges
			for (const auto &e: graph.in_edges(u)) {
				if (!visitor.examine_backwards_edge(e, graph))
					continue;
				auto v = e.source;
				if (colors.get(v) == WHITE) {
					visitor.tree_edge(e, graph);
					colors.put_fast(v, GRAY);
					if (visitor.is_wanted_vertex(v, graph))
						return true;
					if (visitor.discover_vertex(v, graph))
						Q.push(v);
				}
			}

			// forward edges
			for (const auto &e: graph.out_edges(u)) {
				if (!visitor.examine_forwards_edge(e, graph))
					continue;
				auto v = e.target;
				if (colors.get(v) == WHITE) {
					visitor.tree_edge(e, graph);
					colors.put_fast(v, GRAY);
					if (visitor.is_wanted_vertex(v, graph))
						return true;
					if (visitor.discover_vertex(v, graph))
						Q.push(v);
				}
			}

			colors.put_fast(u, BLACK);
			visitor.finish_vertex(u, graph);
		}

		visitor.finished(graph);
		return false;
	}

	template<class Graph, class BFSVisitor>
	bool breadth_first_search_twoway(Graph &graph, BFSVisitor visitor, typename Graph::Vertex source, ColorMap &colors) {
		std::vector<typename Graph::Vertex> sources({source});
		return breadth_first_search_twoway(graph, visitor, sources, colors);
	}



	template<class Graph, class BFSVisitor>
	void breadth_first_search_forward(Graph &graph, BFSVisitor visitor, std::vector<typename Graph::Vertex> sources, ColorMap &colors) {
		if (sources.empty())
			return;
		// Init
		boost::queue<typename Graph::Vertex> Q;
		colors.init();
		for (const auto s: sources) {
			colors.put_fast(s, GRAY);
			if (visitor.discover_vertex(s, graph))
				Q.push(s);
		}


		// Traverse graph
		while (!Q.empty()) {
			auto u = Q.top();
			Q.pop();

			visitor.examine_vertex(u, graph);

			// forward edges
			for (const auto &e: graph.out_edges(u)) {
				if (!visitor.examine_forwards_edge(e, graph))
					continue;
				auto v = e.target;
				if (colors.get(v) == WHITE) {
					visitor.tree_edge(e, graph);
					colors.put_fast(v, GRAY);
					if (visitor.discover_vertex(v, graph))
						Q.push(v);
				}
			}

			colors.put_fast(u, BLACK);
			visitor.finish_vertex(u, graph);
		}

		visitor.finished(graph);
	}

	template<class Graph, class BFSVisitor>
	void breadth_first_search_forward(Graph &graph, BFSVisitor visitor, typename Graph::Vertex source, ColorMap &colors) {
		std::vector<typename Graph::Vertex> sources({source});
		breadth_first_search_forward(graph, visitor, sources, colors);
	}




	template<class Graph>
	struct default_dfs_visitor {
		inline void initialize_vertex(typename Graph::Vertex u, const Graph &g) {}

		inline void start_vertex(typename Graph::Vertex u, const Graph &g) {}

		inline void discover_vertex(typename Graph::Vertex u, const Graph &g) {}

		inline void examine_edge(typename Graph::Edge u, const Graph &g) {}

		inline void tree_edge(typename Graph::Edge u, const Graph &g) {}

		inline void back_edge(typename Graph::Edge u, const Graph &g) {}

		inline void forward_or_cross_edge(typename Graph::Edge u, const Graph &g) {}

		inline void finish_edge(typename Graph::Edge u, const Graph &g) {}

		inline void finish_vertex(typename Graph::Vertex u, const Graph &g) {}
	};

	namespace {
		template<class Graph>
		typename Graph::Vertex get_default_starting_vertex(const Graph &g) {
			if (g.num_vertices() == 0)
				return -1;
			return *g.vertex_set().begin();
		}

		// If the vertex u and the iterators ei and ei_end are thought of as the
		// context of the algorithm, each push and pop from the stack could
		// be thought of as a context shift.
		// Each pass through "while (ei != ei_end)" may refer to the out-edges of
		// an entirely different vertex, because the context of the algorithm
		// shifts every time a white adjacent vertex is discovered.
		// The corresponding context shift back from the adjacent vertex occurs
		// after all of its out-edges have been examined.
		//
		// See http://lists.boost.org/MailArchives/boost/msg48752.php for FAQ.
		template<class Graph, class DFSVisitor>
		void depth_first_visit_impl(Graph &g, typename Graph::Vertex u, DFSVisitor &vis, ColorMap &color) {
			typedef typename Graph::Vertex Vertex;

			typename Graph::EdgeIterator ei, ei_end;
			typedef std::pair<Vertex, std::pair<typename Graph::EdgeIterator, typename Graph::EdgeIterator>> VertexInfo;
			std::vector<VertexInfo> stack;

			// Possible optimization for vector
			//stack.reserve(num_vertices(g));

			color.put_fast(u, GRAY);
			vis.discover_vertex(u, g);
			ei = g.out_edges(u).begin();
			ei_end = g.out_edges(u).end();
			stack.push_back(std::make_pair(u, std::make_pair(ei, ei_end)));

			while (!stack.empty()) {
				VertexInfo &back = stack.back();
				u = back.first;
				ei = back.second.first;
				ei_end = back.second.second;
				stack.pop_back();

				while (ei != ei_end) {
					Vertex v = ei->target;
					vis.examine_edge(*ei, g);
					auto v_color = color.get(v);
					if (v_color == WHITE) {
						vis.tree_edge(*ei, g);
						stack.push_back(std::make_pair(u, std::make_pair(++ei, ei_end)));
						u = v;
						ei = g.out_edges(u).begin();
						ei_end = g.out_edges(u).end();
						color.put_fast(u, GRAY);
						vis.discover_vertex(u, g);
					} else if (v_color == GRAY) {
						vis.back_edge(*ei, g);
						++ei;
					} else {
						vis.forward_or_cross_edge(*ei, g);
						++ei;
					}
				}
				color.put_fast(u, BLACK);
				vis.finish_vertex(u, g);
			}
		}
	}

	template<class Graph, class DFSVisitor>
	void depth_first_search(Graph &graph, DFSVisitor visitor, typename Graph::Vertex source, ColorMap &color) {
		color.init();
		for (auto u: graph.vertex_set()) {
			visitor.initialize_vertex(u, graph);
		}

		if (source != graph.null_vertex() && source != get_default_starting_vertex(graph)) {
			visitor.start_vertex(source, graph);
			depth_first_visit_impl(graph, source, visitor, color);
		}

		for (auto u: graph.vertex_set()) {
			if (color.get(u) == WHITE) {
				visitor.start_vertex(u, graph);
				depth_first_visit_impl(graph, u, visitor, color);
			}
		}
	}


	/**
	 * Visit an acyclic directed graph in bottom-up fashion.
	 * If the graph is not acyclic, not all nodes will be visited.
	 * @tparam Graph
	 * @tparam Visitor
	 * @param graph an acyclic graph
	 * @param visitor callback object with () operator method. Type: void(Vertex, Graph&). For example a lambda.
	 */
	template<class Graph, class Visitor>
	void bottom_up_traversal(Graph &graph, Visitor visitor) {
		AbstractMap<long> resolved(graph);
		resolved.init(0l);
		boost::queue<typename Graph::Vertex> Q;

		// Init - all vertices with no remaining
		for (auto v: graph.vertex_set()) {
			if (graph.out_edges(v).size() == 0) {
				Q.push(v);
			}
		}

		// Vertex is added to queue once all its dependencies (out-edges) have been visited
		while (!Q.empty()) {
			auto v = Q.top();
			Q.pop();

			visitor(v, graph);

			for (auto &e: graph.in_edges(v)) {
				// u has one more resolved out_edge
				auto u = e.source;
				long r = resolved.get(u) + 1;
				resolved.put(u, r);
				// if all are resolved, continue
				if (r == graph.out_edges(u).size()) {
					Q.push(u);
				}
			}
		}
	}

}

#endif
