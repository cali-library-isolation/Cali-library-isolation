#include "PDG.h"
#include <iostream>
#include <boost/pending/queue.hpp>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/GlobalVariable.h>


using namespace boost;

void PDG::createSubnodes(Vertex parent, int limit) {
	auto type = (*this)[parent].type;

	// Check for constant things
	// Global constants get constant subnodes.
	auto ins = (*this)[parent].instruction;
	auto isConstant = (*this)[parent].constant || (ins && llvm::isa<llvm::GlobalVariable>(ins) && llvm::cast<llvm::GlobalVariable>(ins)->isConstant());

	// Handle an array like a variable of that type
	if (type->isArrayTy()) {
		type = type->getArrayElementType();
	}

	if (type->isPointerTy()) {
		PDGNode n;
		n.function = (*this)[parent].function;
		n.type = type->getPointerElementType();
		n.subnode = true;
		n.constant = isConstant;
		auto newNode = pdg_add_vertex(n);
		add_edge(parent, newNode, PDGEdge{.type= PDGEdge::Deref, .index=0});
		if (limit > 0)
			createSubnodes(newNode, limit - 1);

	} else if (type->isStructTy()) {
		if (type->getStructNumElements() > max_parts + 1) {
			// llvm::outs() << "[] Dropping struct information: struct has " << type->getStructNumElements() << " members.\n";
		}
		for (unsigned int i = 0; i < type->getStructNumElements(); i++) {
			PDGNode n;
			n.function = (*this)[parent].function;
			n.type = type->getStructElementType(i);
			n.subnode = true;
			n.constant = isConstant;
			if (i < max_parts) {
				auto newNode = pdg_add_vertex(n);
				add_edge(parent, newNode, PDGEdge{.type= PDGEdge::Part, .index=i});
				createSubnodes(newNode, limit - 1);
			} else {
				auto optNode = getPartOpt(parent, i);
				auto newNode = optNode ? *optNode : pdg_add_vertex(n);
				add_edge(parent, newNode, PDGEdge{.type= PDGEdge::Part, .index=max_parts});
				if (!optNode)
					createSubnodes(newNode, limit - 1);
			}
		}
	}
}

Vertex PDG::createVertex(llvm::Value *value, llvm::Function *function) {
	if (!function && llvm::isa<llvm::Instruction>(value)) {
		function = llvm::cast<llvm::Instruction>(value)->getFunction();
	}
	PDGNode n;
	n.instruction = value;
	// Functions are always contained in themselves - no matter where they occur first
	n.function = (value && llvm::isa<llvm::Function>(value)) ? llvm::cast<llvm::Function>(value) : function;
	n.type = value->getType();
	n.source = false; // set by PDGCreationPass
	n.source_type = 0;
	auto newNode = value_to_vertex[value] = pdg_add_vertex(n);
	if (!llvm::isa<llvm::Function>(value))
		createSubnodes(newNode, subnode_limit);
	return newNode;
}

Vertex PDG::createDummyVertex(llvm::Value *value, llvm::Function *function) {
	if (!function && value && llvm::isa<llvm::Instruction>(value)) {
		function = llvm::cast<llvm::Instruction>(value)->getFunction();
	}
	PDGNode n;
	n.instruction = value;
	// Functions are always contained in themselves - no matter where they occur first
	n.function = (value && llvm::isa<llvm::Function>(value)) ? llvm::cast<llvm::Function>(value) : function;
	n.type = value->getType();
	n.source = false; // set by PDGCreationPass
	n.source_type = 0;
	n.dummy = true;
	return pdg_add_vertex(n);
}

boost::optional<Vertex> PDG::getVertexOpt(const llvm::Value *value) {
	auto it = value_to_vertex.find(value);
	if (it != value_to_vertex.end())
		return it->second;
	return {};
}

Vertex PDG::getDeref(Vertex v) {
	for (const auto &e: out_edges(v)) {
		if ((*this)[e].type == PDGEdge::Deref)
			return e.target;
	}
	throw std::runtime_error("Vertex can't be dereferenced");
}

boost::optional<Vertex> PDG::getDerefOpt(Vertex v) {
	for (const auto &e: out_edges(v)) {
		if ((*this)[e].type == PDGEdge::Deref)
			return e.target;
	}
	return {};
}

boost::optional<Vertex> PDG::getPartOpt(Vertex v, unsigned int index) {
	if (index > max_parts) index = max_parts;
	for (const auto &e: out_edges(v)) {
		if ((*this)[e].type == PDGEdge::Part && (*this)[e].index == index)
			return e.target;
	}
	return {};
}

boost::optional<Vertex> PDG::getParamOpt(Vertex v, unsigned int index) {
	for (const auto &e: out_edges(v)) {
		if ((*this)[e].type == PDGEdge::Param && (*this)[e].index == index)
			return e.target;
	}
	return {};
}

boost::optional<Vertex> PDG::getCallParamOpt(Vertex v, unsigned int index) {
	for (const auto &e: in_edges(v)) {
		if ((*this)[e].type == PDGEdge::Param && (*this)[e].index == index)
			return e.source;
	}
	return {};
}

boost::optional<Vertex> PDG::getReturnOpt(Vertex v) {
	for (const auto &e: out_edges(v)) {
		if ((*this)[e].type == PDGEdge::Return)
			return e.target;
	}
	return {};
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-loop-convert"

void PDG::addSubnodeEdges(Vertex v1, Vertex v2, int constantLevels) {
	if (v1 == v2)
		return;

	// No data flow towards constant edges
	if ((*this)[v2].constant)
		return;

	// Add data flow
	add_edge(v1, v2, PDGEdge{.type=PDGEdge::Data, .index=0});

	// Join subnodes
	for (size_t i = 0; i < out_edges(v1).size(); i++) {
		const auto e = out_edges(v1)[i];
		auto v1s = e.target;
		switch ((*this)[e].type) {
			case PDGEdge::Deref: {
				auto v2s = getDerefOpt(v2);
				if (v2s && v1s != *v2s) {
					if (constantLevels <= 0) {
						uniteSubnodes(v1s, *v2s);
					} else {
						addSubnodeEdges(v1s, *v2s, constantLevels - 1);
					}
				}
				break;
			}
			case PDGEdge::Part: {
				auto v2s = getPartOpt(v2, (*this)[e].index);
				if (v2s && (v1 != v1s || v2 != v2s))
					addSubnodeEdges(v1s, *v2s, constantLevels);
				break;
			}
			default:
				break;
		}
	}
}

#pragma clang diagnostic pop

void PDG::uniteSubnodes(Vertex v1, Vertex v2) {
	if (((*this)[v1].instruction && (*this)[v2].instruction) ||
		(*this)[v1].function != (*this)[v2].function ||
		(*this)[v1].constant || (*this)[v2].constant) {
		// Check if we already united this pair
		if (v1 > v2) swap(v1, v2);
		auto it = united_nodes.find({v1, v2});
		if (it != united_nodes.end())
			return;
		united_nodes.insert({v1, v2});

		// instructions can't be joined, use two-way dataflow, which should be somehow equivalent
		addSubnodeEdges(v1, v2);
		addSubnodeEdges(v2, v1);
		return;
	}

	std::vector<std::tuple<Vertex, Vertex, PDGEdge>> new_edges;

	boost::queue<std::pair<Vertex, Vertex>> Q;
	Q.push({v1, v2});

	std::map<Vertex, Vertex> old_to_new; // removed vertex => replacement vertex
	// order of vertices: vertices with instruction are lower than ones without, vertices with lower index are lower.
	// in case of doubt, higher vertex gets removed.
	auto swapLowerFirst = [this](Vertex &v1, Vertex &v2) {
		if (!(*this)[v1].instruction && ((*this)[v2].instruction || v1 > v2))
			swap(v1, v2);
	};

	while (!Q.empty()) {
		boost::tie(v1, v2) = Q.top();
		Q.pop();
		if (v1 == v2) continue;
		swapLowerFirst(v1, v2);
		// v2 should be removed, usages should be replaced with v1

		// check & update old-to-new map
		std::pair<std::map<Vertex, Vertex>::iterator, bool> insert_result;
		while (!(insert_result = old_to_new.insert({v2, v1})).second) { // while v2 in old_to_new:
			v2 = insert_result.first->second; // v2 = old_to_new[v2]
			if (v1 == v2) break;
			swapLowerFirst(v1, v2);
		}
		if (v1 == v2) continue;
		old_to_new[v2] = v1;

		// join node attributes
		auto &v1Property = (*this)[v1];
		auto &v2Property = (*this)[v2];
		if (!v1Property.source && v2Property.source) {
			v1Property.source = v2Property.source;
			v1Property.source_type = v2Property.source_type;
		}
		if (!v1Property.ipcsink && v2Property.ipcsink)
			v1Property.ipcsink = v2Property.ipcsink;
		if (!v1Property.reaches_ipc_sink && v2Property.reaches_ipc_sink)
			v1Property.reaches_ipc_sink = v1Property.reaches_ipc_sink;
		if (!v1Property.filedescriptor && v2Property.filedescriptor)
			v1Property.filedescriptor = v2Property.filedescriptor;
		if (v2Property.instruction) {
			if (v1Property.instruction) llvm::errs() << *v1Property.instruction;
			else llvm::errs() << "(null)";
			llvm::errs() << "  ==  ";
			if (v2Property.instruction) llvm::errs() << *v2Property.instruction;
			else llvm::errs() << "(null)";
			llvm::errs() << "\n";
			throw std::runtime_error("Can't remove vertices with instruction");
		}

		boost::optional<Vertex> v1Deref;
		boost::optional<Vertex> v1Part[8];
		for (const auto &e: out_edges(v1)) {
			if ((*this)[e].type == PDGEdge::Deref) {
				v1Deref = e.target;
			} else if ((*this)[e].type == PDGEdge::Part && (*this)[e].index < 8) {
				v1Part[(*this)[e].index] = e.target;
			}
		}

		for (const auto &e: out_edges(v2)) {
			auto v2s = e.target;
			switch ((*this)[e].type) {
				case PDGEdge::Deref: {
					if (v1Deref) {
						// remove this edge
						Q.push({*v1Deref, v2s});
					} else {
						// change source of this edge to v1
						new_edges.emplace_back(v1, v2s, (*this)[e]);
					}
					break;
				}
				case PDGEdge::Part: {
					auto v1s = (*this)[e].index < 8 ? v1Part[(*this)[e].index] : getPartOpt(v1, (*this)[e].index);
					if (v1s) {
						// remove this edge
						Q.push({*v1s, v2s});
					} else {
						// change source of this edge to v1
						new_edges.emplace_back(v1, v2s, (*this)[e]);
					}
					break;
				}
				default:
					// change source of this edge to v1
					new_edges.emplace_back(v1, v2s, (*this)[e]);
					break;
			}
		}

		for (const auto &e: in_edges(v2)) {
			// set target of e2 to v1
			new_edges.emplace_back(e.source, v1, (*this)[e]);
		}

		// add edges (in that little moment where no iterators run)
		for (auto new_edge: new_edges) {
			add_edge(std::get<0>(new_edge), std::get<1>(new_edge), std::get<2>(new_edge));
		}
		new_edges.clear();
		(*this)[v2].ipcsink = false;
		(*this)[v2].reaches_ipc_sink = false;
		pdg_remove_vertex(v2);
	};
}


void PDG::removeFunction(llvm::Function *function) {
	auto set = getVerticesForFunction(function);
	for (auto v: set) {
		auto it = value_to_vertex.find(vertices[v].property.instruction);
		if (it != value_to_vertex.end() && it->second == v) {
			value_to_vertex.erase(it);
		}
		remove_vertex(v);
	}
	set.clear();
}

boost::optional<Vertex> PDG::getMemoryForSource(Vertex src) {
	auto &n = (*this)[src];
	switch (n.source_type) {
		case 1:
			// malloc style
			return getDerefOpt(src);
		case 2: {
			auto v = getCallParamOpt(src, 0);
			if (v) {
				v = getDerefOpt(*v);
				if (v)
					return getDerefOpt(*v);
			}
		}
		default:
			return {};
	}
	return 0;
}

void PDG::checkIfFDMarkerIsInteresting(Vertex v) {
	for (const auto &e: in_edges(v)) {
		if ((*this)[e].type == PDGEdge::Part) {
			auto st = (*this)[e.source].type;
			if (st && st->isStructTy()) {
				// look up cluster
				auto cluster = structClusters->find((const llvm::StructType *) st);
				if (cluster != structClusters->end()) {
					structClusterToKnownFDFields[cluster->second].insert((*this)[e].index);
				}
			}
		}
	}
}

std::set<Vertex> &PDG::getVerticesForFunction(llvm::Function *function) {
	return function_to_vertices[function];
}
