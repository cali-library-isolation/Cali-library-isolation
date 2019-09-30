#include "PDGWriterPass.h"
#include "PDG.h"
#include "PDGCreationPass.h"
#include <iostream>
#include <fstream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <boost/graph/graphviz.hpp>
#include "../graph/dotwriter.h"
#include "../stdlib-is-shit.h"


using namespace std;

struct PDGNodeWriter {
	template<class Vertex>
	inline void operator()(std::ostream &out, PDG &pdg, Vertex v) {
		PDGNode &node = pdg[v];
		std::string str;
		llvm::raw_string_ostream rso(str);
		if (node.instruction) {
			if (llvm::isa<llvm::Function>(node.instruction)) {
				rso << "Function \"" << node.instruction->getName() << "\"";
			} else {
				node.instruction->print(rso);
			}
			rso << " : ";
		} else {
			rso << "[sub] Type: ";
		}
		node.type->print(rso);
		if (node.constant) rso << " const";
		rso.flush();
		out << " [id=v" << v << " label=\"";
		if (node.function) out << node.function->getName().str() << " - "; //tmp
		out << "#" << v;
		if (node.subnode) out << " (sub)";
		out << ": \\n" << replaceAll(replaceAll(replaceAll(str.substr(0, 200), "\\", "\\\\"), "\"", "\\\""), "\n", "\\n") << "\"";

		if (node.instruction && llvm::isa<llvm::Function>(node.instruction)) out << " style=filled fillcolor=blue4 fontcolor=white";
		else if (node.source) out << " style=filled fillcolor=darkolivegreen2";
		else if (node.ipcsink) out << " style=filled fillcolor=coral";
		else if (!node.subnode) out << " style=filled fillcolor=gold";
		else out << " style=filled fillcolor=azure";
		if (node.filedescriptor) out << " penwidth=3 color=darkorchid2";
		else if (node.reaches_ipc_sink) out << " penwidth=2 color=firebrick";
		else if (node.ipcfunction) out << " penwidth=2 color=blue4";
		out << "]";
	};
};

struct PDGEdgeWriter {
	template<class Edge>
	inline void operator()(std::ostream &out, const PDG &pdg, Edge &e) {
		// just an example, showing that local options override global
		//out << " [color=purple]" << std::endl;
		out << " [label=\"";
		switch (e.property.type) {
			case PDGEdge::Control:
				out << "ctrl";
				break;
			case PDGEdge::Data:
				out << "data";
				break;
			case PDGEdge::Equals:
				out << "=";
				break;
			case PDGEdge::Deref:
				out << "*";
				break;
			case PDGEdge::Part:
				out << ". [" << e.property.index << "]";
				break;
			case PDGEdge::Param:
				out << "param [" << e.property.index << "]";
				break;
			case PDGEdge::Return:
				out << "ret";
				break;
			case PDGEdge::Summary_Data:
				out << "s[data]\" penwidth=2 color=\"forestgreen";
				break;
			case PDGEdge::Summary_Equals:
				out << "s[=]\" penwidth=2 color=\"forestgreen";
				break;
		}
		out << "\"]";
	};
};


struct PDGFunctionFilter {
	std::set<llvm::Function *> functions;

	inline bool operator()(PDG &pdg, Vertex v) {
		// Ignore debug info
		if (pdg[v].instruction && llvm::isa<llvm::CallInst>(pdg[v].instruction) &&
			llvm::cast<llvm::CallInst>(pdg[v].instruction)->getCalledFunction() &&
			llvm::cast<llvm::CallInst>(pdg[v].instruction)->getCalledFunction()->getName().startswith("llvm.dbg"))
			return false;
		// Include globals and values associated with a function from our set
		return !pdg[v].function || pdg[v].instruction == pdg[v].function || functions.find(pdg[v].function) != functions.end();
	}
};




char PDGWriterPass::ID = 0;

static llvm::RegisterPass<PDGWriterPass> writerReg("pdg-writer", "PDG Dump", false, false);

void PDGWriterPass::getAnalysisUsage(llvm::AnalysisUsage &Info) const {
	Info.setPreservesCFG();
	Info.setPreservesAll();
	Info.addRequired<PDGCreationPass>();
}

bool PDGWriterPass::runOnModule(llvm::Module &M) {
	auto &graph = getAnalysis<PDGCreationPass>().graph;
	cout << "--- PDG Writer starts ---" << endl;
	cout << "module: " << M.getName().str() << endl;
	cout << graph.num_vertices() << " vertices" << endl;
	cout << graph.num_edges() << " edges" << endl;

	auto legend = "subgraph cluster_legend{\n"
				  "\trankdir=TB;\n"
				  "\tlabel=\"Legend\";\n"
				  "\tgraph[style=solid nodesep=6 ranksep=6 pos=\"0,0!\"];\n"
				  "\n"
				  "\tfunc [label=\"Function\" style=filled fillcolor=blue4 fontcolor=white];\n"
				  "\tsource [label=\"Source\" style=filled fillcolor=darkolivegreen2];\n"
				  "\tipcsink [label=\"IPC-Sink\" style=filled fillcolor=coral];\n"
				  "\tmainnode [label=\"Instruction node\" style=filled fillcolor=gold];\n"
				  "\tsubnode [label=\"Subnode\" style=filled fillcolor=azure];\n"
				  "\tripc [label=\"Reaching IPC\" penwidth=2 color=firebrick];\n"
				  "\tipcfunc [label=\"IPC function\" penwidth=2 color=blue4];\n"
				  "\t{\n"
				  "\t\trank=same;\n"
				  "\t\ta [style=invis];\n"
				  "\t\tb [style=invis];\n"
				  "\t}\n"
				  "\ta -> b [label=\"Summary Edge\" penwidth=2 color=\"forestgreen\"];\n"
				  "}";


	{
		ofstream f(M.getName().str() + ".dot", ios::out | ios::binary);
		// basegraph::dot_write_graph(f, graph, PDGNodeWriter(), PDGEdgeWriter(), PDGFunctionFilter());

		PDGFunctionFilter filter;
		std::vector<string> functionNames{
				"InvokePostscriptDelegate",
				"StringToArgv",
				"GetConfigurePaths",
				"GetLocaleOptions",
				"ConstantString",
				// socat
				"_xioopen_openssl_prepare",
				"sycSSL_CTX_load_verify_locations",
				"retropt_string",
				"strdup",
				"cv_newline",
				"SSL_set_fd",
				"sycSSL_set_fd",
				"xioSSL_set_fd",
				"xiosocket",
				// filezilla
				"_ZN13CQueueStorageC2Ev",
				"_ZNKSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE5c_strEv",
				"_ZN2fz7to_utf8ERKNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEEE",
				"_ZN13CQueueStorage4Impl17PrepareStatementsEv",
				"_ZN13CQueueStorage4Impl16PrepareStatementERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE",
				"_ZN13CQueueStorage4Impl4BindEP12sqlite3_stmtiPKc",
				"_ZN13CQueueStorage4Impl4BindEP12sqlite3_stmtiRKNSt7__cxx1112basic_stringIwSt11char_traitsIwESaIwEEE",
				"_ZN13CQueueStorage4Impl8SaveFileERK9CFileItem",
				"_ZN13CQueueStorage4Impl10SaveServerERK11CServerItem",
				"_ZN13CQueueStorage4Impl13SaveDirectoryERK11CFolderItem"
		};
		for (const auto &fn: functionNames) {
			filter.functions.insert(M.getFunction(fn));
			filter.functions.insert(M.getFunction("__ipc_specialized_" + fn));
		}

		std::vector<Vertex> sources;
		for (const auto func: filter.functions) {
			if (func == nullptr)
				continue;
			auto v = graph.getVertex(func);
			sources.push_back(v);
			for (const auto &bb: *func) {
				for (const auto &ins: bb) {
					if (llvm::isa<llvm::CallInst>(ins)) {
						auto &call = llvm::cast<llvm::CallInst>(ins);
						Vertex vertex = graph.getVertex((llvm::Value *) &call);
						sources.push_back(vertex);
					}
					else if (llvm::isa<llvm::InvokeInst>(ins)) {
						auto &call = llvm::cast<llvm::InvokeInst>(ins);
						Vertex vertex = graph.getVertex((llvm::Value *) &call);
						sources.push_back(vertex);
					}
				}
			}
		}
		basegraph::dot_bfs_write_graph(f, graph, sources, PDGNodeWriter(), PDGEdgeWriter(), filter, legend);
	}

	if (runDot) {
		string cmd = "dot -Tsvg \"" + M.getName().str() + ".dot\" -o \"" + M.getName().str() + ".svg\"";
		cout << "> " << cmd << endl;
		if (system(cmd.c_str())) perror("system(dot)");
		cout << "Created " << M.getName().str() + ".svg" << endl;
	} else {
		cout << "Created " << M.getName().str() + ".dot" << endl;
	}

	return false;
}
