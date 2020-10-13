#ifndef LLVMREWRITER_PDG_H
#define LLVMREWRITER_PDG_H


#include <llvm/IR/Function.h>
#include <boost/graph/properties.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "../graph/basegraph.h"
#include "../cali_linker/configfile.h"

struct PDGNode {
	llvm::Function *function = nullptr; // Do not update "function" directly!
	llvm::Value *instruction = nullptr;
	llvm::Type *type = nullptr;
	bool subnode = false; // true if node does not correspond to an instruction / value, but denotes a part of a value (struct member, pointer destination, ...)
	bool dummy = false; // true if node is not really part of an instruction (example: cloned indirect calls from function summaries)
	bool source = false; // true if node is memory source
	bool ipcsink = false; // true for nodes that cross the ipc bridge (e.g: arguments of ipcfunction calls)
	bool reaches_ipc_sink = false;  // memory allocating nodes that reach an ipcsink node (via data-flow)
	bool ipcfunction = false; // true for functions in an other context
	bool constant = false; // true if this node is immutable (no data edges should point towards it)
	bool filedescriptor = false; // true if this node is a filedescriptor
	char source_type = 0; // 0 = no source, 1 = malloc style/variable, 2 = posix_memalign style
};

struct PDGEdge {
	enum PDGEdgeType {
		Control,
		Data,
		Equals,
		Part,
		Deref,
		Param,
		Return,
		Invocation, // function pointer to call instruction invoking it
		Summary_Data,
		Summary_Equals,
		Summary_Invocation
	};
	PDGEdgeType type;
	unsigned int index = 0; // index of struct / param
};

inline bool operator==(const PDGEdge &lhs, const PDGEdge &rhs) {
	return lhs.type == rhs.type && lhs.index == rhs.index;
}


typedef basegraph::BaseGraph<PDGNode, PDGEdge> PDG_Proto;
typedef PDG_Proto::Vertex Vertex;
typedef PDG_Proto::Edge Edge;

class PDG : public PDG_Proto {
public:
	std::map<const llvm::Value *, Vertex> value_to_vertex;

	const std::map<const llvm::StructType *, int> *structClusters;
	std::map<int, std::set<unsigned int>> structClusterToKnownFDFields;

	unsigned int max_parts = 32;
	unsigned int subnode_limit = 5;
private:
	std::set<std::pair<Vertex, Vertex>> united_nodes; // nodes that should be united, but needed the twoway-data-fallback
	std::map<llvm::Function *, std::set<Vertex>> function_to_vertices;

public:

	explicit PDG(const YamlConfig *config) {
		if (config) {
			max_parts = config->limit_struct_parts;
			subnode_limit = config->limit_subnode_depth;
		}
	}

	inline Vertex pdg_add_vertex(PDGNode prop) {
		auto v = add_vertex(prop);
		function_to_vertices[prop.function].insert(v);
		return v;
	}

	inline void pdg_remove_vertex(Vertex v) {
		function_to_vertices[(*this)[v].function].erase(v);
		remove_vertex(v);
	}

	inline void updateFunction(Vertex v, llvm::Function* function) {
		function_to_vertices[(*this)[v].function].erase(v);
		(*this)[v].function = function;
		function_to_vertices[function].insert(v);
	}

	/**
	 * Create the structural representation of a Value (using Deref and Part edges)
	 * @param parent
	 * @param limit
	 */
	void createSubnodes(Vertex parent, int limit);

	/**
	 * Create the vertex corresponding to a given LLVM value, including structural representation
	 * @param value
	 * @param function
	 * @return
	 */
	Vertex createVertex(llvm::Value *value, llvm::Function *function);

	/**
	 * Create a "dummy" vertex that has a link to a value, but the value does not link to it. There can be multiple dummy vertices for each value.
	 * @param value
	 * @param function
	 * @return
	 */
	Vertex createDummyVertex(llvm::Value *value, llvm::Function *function);

	/**
	 * The function parameter is optional. If the value has no associated function, this one will be used.
	 * @param value
	 * @param function
	 * @return The vertex corresponding to a given LLVM value
	 */
	inline Vertex getVertex(llvm::Value *value, llvm::Function *function = nullptr) {
		auto it = value_to_vertex.find(value);
		if (it != value_to_vertex.end()) {
			if (function != nullptr && (*this)[it->second].function == nullptr) {
				updateFunction(it->second, function);
			}
			return it->second;
		}
		return createVertex(value, function);
	}

	boost::optional<Vertex> getVertexOpt(const llvm::Value *value);

	Vertex getDeref(Vertex v);

	boost::optional<Vertex> getDerefOpt(Vertex v);

	boost::optional<Vertex> getPartOpt(Vertex v, unsigned int index);

	/**
	 * Get a forward "param" edge (if exists). That means: Function => Parameter, Argument => Call
	 * @param v
	 * @param index
	 * @return
	 */
	boost::optional<Vertex> getParamOpt(Vertex v, unsigned int index);

	/**
	 * Get a backwards "param" edge (if exists). That means: Parameter => Function, Call => Call Argument
	 * @param v
	 * @param index
	 * @return
	 */
	boost::optional<Vertex> getCallParamOpt(Vertex v, unsigned int index);

	boost::optional<Vertex> getReturnOpt(Vertex v);

	std::set<Vertex> &getVerticesForFunction(llvm::Function *function);

	/**
	 * Create structural edges that represent data flow / equality from v1 to v2.
	 * @param v1
	 * @param v2
	 * @param constantLevels number of additional levels that should be constant (non-unified).
	 * 0 => everything after a * gets two-directional dataflow / unified
	 * 1 => everything behind two * gets unified
	 */
	void addSubnodeEdges(Vertex v1, Vertex v2, int constantLevels = 0);

	/**
	 * Create "equals" relations between two vertices and their subnodes. That means: unite the nodes (to be one finally). v1 survives.
	 * @param v1
	 * @param v2
	 */
	void uniteSubnodes(Vertex v1, Vertex v2);

	void removeFunction(llvm::Function *function);

	boost::optional<Vertex> getMemoryForSource(Vertex src);

	void checkIfFDMarkerIsInteresting(Vertex v);
};

#endif //LLVMREWRITER_PDG_H