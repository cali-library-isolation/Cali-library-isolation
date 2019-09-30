#ifndef LLVMREWRITER_SCC_ALGORITHMS_H
#define LLVMREWRITER_SCC_ALGORITHMS_H

#include "abstract_map.h"
#include "algorithms.h"

namespace basegraph {
	typedef AbstractMap<long> ComponentMap;

	namespace {
		//*
		template<class Graph>
		class tarjan_scc_visitor : public default_dfs_visitor<Graph> {
			long &c;
			ComponentMap &comp;
			AbstractMap<typename Graph::Vertex> &root;
			AbstractMap<long> &discover_time;
			long &dfs_time;
			std::stack<typename Graph::Vertex> &s;

		public:
			tarjan_scc_visitor(ComponentMap &comp_map, long &cref,
							   AbstractMap<typename Graph::Vertex> &rootmap,
							   AbstractMap<long> &d,
							   std::stack<typename Graph::Vertex> &_s,
							   long &dref)
					: comp(comp_map), c(cref), root(rootmap), discover_time(d), s(_s), dfs_time(dref) {}

			inline void discover_vertex(typename Graph::Vertex v, const Graph &) {
				root.put(v, v);
				comp.put(v, std::numeric_limits<long>::max());
				discover_time.put(v, dfs_time++);
				s.push(v);
			}

			inline void finish_vertex(typename Graph::Vertex v, Graph &g) {
				for (auto &e: g.out_edges(v)) {
					auto w = e.target;
					if (comp.get(w) == std::numeric_limits<long>::max())
						root.put(v, this->min_discover_time(root.get(v), root.get(w)));
				}
				if (root.get(v) == v) {
					typename Graph::Vertex w;
					int counter = 0;
					do {
						w = s.top();
						s.pop();
						comp.put(w, c);
						root.put(w, v);
						counter++;
					} while (w != v);
					++c;
				}
			}

		private:
			template<typename Vertex>
			Vertex min_discover_time(Vertex u, Vertex v) {
				return discover_time.get(u) < discover_time.get(v) ? u : v;
			}
		};


		template<class Graph>
		long strong_components_impl(Graph &g, ComponentMap &comp) {
			long total = 0;
			std::stack<typename Graph::Vertex> s;

			AbstractMap<typename Graph::Vertex> root(g);
			root.init(g.null_vertex());
			AbstractMap<long> discover_time(g);

			long d = 0;
			tarjan_scc_visitor<Graph> vis(comp, total, root, discover_time, s, d);

			ColorMap color(g);
			depth_first_search(g, vis, g.null_vertex(), color);
			return total;
		}

	}

	template<class Graph>
	long strong_components(Graph &g, ComponentMap &comp) {
		return strong_components_impl(g, comp);
	}

	template<class InGraph, class OutGraph>
	long strong_components_graph(InGraph &inGraph, OutGraph &outGraph) {
		ComponentMap components(inGraph);
		long size = strong_components(inGraph, components);

		// 1 new vertex for every SCC
		for (long i = 0; i < size; i++)
			outGraph.add_vertex({});
		for (auto v1: inGraph.vertex_set()) {
			auto v2 = components.get(v1);
			// Each new vertex get list of old vertices in SCC
			outGraph[v2].push_back(v1);
			// Edges
			for (auto &e: inGraph.out_edges(v1)) {
				auto target2 = components.get(e.target);
				if (v2 != target2)
					outGraph.add_edge(v2, target2, {});
			}
		}

		return size;
	}
}

#endif //LLVMREWRITER_SCC_ALGORITHMS_H
