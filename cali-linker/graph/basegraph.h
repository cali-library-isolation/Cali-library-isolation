#ifndef LLVMREWRITER_BASEGRAPH_H
#define LLVMREWRITER_BASEGRAPH_H


#include <vector>
#include <stack>
#include "unique_vector.h"

namespace basegraph {

	template<typename Vertex, class EdgeProperty>
	struct EdgeImplBase {
		Vertex source;
		Vertex target;
		EdgeProperty property;
	};

	template<typename Vertex, class EdgeProperty>
	inline bool operator==(const EdgeImplBase<Vertex, EdgeProperty> &lhs, const EdgeImplBase<Vertex, EdgeProperty> &rhs) {
		return lhs.source == rhs.source && lhs.target == rhs.target && lhs.property == rhs.property;
	}

	template<typename Vertex, class VertexProperty, class EdgeProperty>
	struct VertexImplBase {
		unique_vector<EdgeImplBase<Vertex, EdgeProperty>> out_edges;
		unique_vector<EdgeImplBase<Vertex, EdgeProperty>> in_edges;
		VertexProperty property;
		bool empty = false;
	};

	template<class VertexProperty, class EdgeProperty>
	class BaseGraph {
	public:
		typedef long unsigned int Vertex;
		typedef VertexImplBase<Vertex, VertexProperty, EdgeProperty> VertexImpl;
		typedef EdgeImplBase<Vertex, EdgeProperty> Edge;
		typedef typename unique_vector<Edge>::iterator EdgeIterator;

	private:
		size_t size = 0;
		size_t edge_size = 0;
		size_t max_capacity = 0;
		std::stack<Vertex> free_vertices;

	public:
		inline Vertex null_vertex() const {
			return -1;
		}

		class VertexIterator {
			Vertex v;
		public:
			explicit VertexIterator(Vertex v) : v(v) {}

			const VertexIterator operator++(int) { return v++; }

			VertexIterator &operator++() {
				++v;
				return *this;
			}

			Vertex operator*() const { return v; }

			// pointer   operator->() const                    { return pos_; }
			// VertexIterator  operator+ (int v)   const { return pos_ + v; }
			bool operator==(const VertexIterator &rhs) const { return v == rhs.v; }

			bool operator!=(const VertexIterator &rhs) const { return v != rhs.v; }
		};

		struct VertexSet {
			VertexIterator _end;

			auto begin() {
				return VertexIterator(0);
			}

			auto end() {
				return _end;
			}
		};

		// Storage: Vertex with adjacency lists
		std::vector<VertexImpl> vertices;

		inline VertexSet vertex_set() const {
			return {._end=VertexIterator(vertices.size())};
		}

		// Compatibility - [] operators for property access
		inline VertexProperty &operator[](Vertex v) {
			return vertices[v].property;
		}

		inline EdgeProperty &operator[](Edge &edge) const {
			return edge.property;
		}

		inline const EdgeProperty &operator[](const Edge &edge) {
			return edge.property;
		}

		// Iterate edges
		inline auto &out_edges(Vertex v) {
			return vertices[v].out_edges;
		}

		inline auto &in_edges(Vertex v) {
			return vertices[v].in_edges;
		}

		// sizes
		inline size_t num_vertices() const {
			return size;
		}

		inline size_t num_edges() const {
			return edge_size;
		}

		/**
		 * Capacity = number of elements in the vertices vector = highest vertex id -1.
		 * @return
		 */
		inline size_t capacity() const {
			return vertices.size();
		}

		inline size_t max_vertex_id() {
			return max_capacity;
		}

		// Add / remove vertices and edges

		inline Vertex add_vertex(VertexProperty prop) {
			size++;
			while (!free_vertices.empty()) {
				auto v = free_vertices.top();
				free_vertices.pop();
				if (v <= vertices.size()) {
					vertices[v].empty = false;
					vertices[v].property = prop;
					return v;
				}
			}

			auto v = vertices.size();
			vertices.emplace_back();
			vertices[v].property = prop;
			return v;
		}

		inline void add_edge(Vertex v1, Vertex v2, EdgeProperty edge) {
			Edge e{.source=v1, .target=v2, .property=edge};
			if (vertices[v1].out_edges.insert(e) && vertices[v2].in_edges.insert(e))
				edge_size++;
		}

		inline void add_edge_unique(Vertex v1, Vertex v2, EdgeProperty edge) {
			Edge e{.source=v1, .target=v2, .property=edge};
			if (vertices[v1].out_edges.insert_unique(e) && vertices[v2].in_edges.insert_unique(e))
				edge_size++;
		}

		inline void remove_vertex(Vertex v) {
			edge_size -= vertices[v].in_edges.size();
			edge_size -= vertices[v].out_edges.size();
			for (const auto &e: vertices[v].in_edges) {
				vertices[e.source].out_edges.remove(e); //TODO can be more efficient
			}
			for (const auto &e: vertices[v].out_edges) {
				vertices[e.target].in_edges.remove(e); //TODO can be more efficient
			}
			vertices[v].in_edges.clear();
			vertices[v].out_edges.clear();
			vertices[v].empty = true;
			size--;
			free_vertices.push(v);
		}
	};

};


#endif //LLVMREWRITER_BASEGRAPH_H
