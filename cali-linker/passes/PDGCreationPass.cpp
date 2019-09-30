#include <iostream>
#include <fstream>
#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>
#include <llvm/Transforms/Utils/UnifyFunctionExitNodes.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Operator.h>
#include "PDGCreationPass.h"
#include "PDGUtilities.h"
#include "../cali_linker/debug.h"

using namespace llvm;

// Utility functions

namespace {
	bool isIgnoredInstruction(Value *value) {
		if (isa<CallInst>(value)) {
			if (cast<CallInst>(value)->getCalledFunction()) {
				auto name = cast<CallInst>(value)->getCalledFunction()->getName();
				return name.startswith("llvm.dbg.") || name.startswith("llvm.lifetime.");
			}
		}
		if (isa<InvokeInst>(value)) {
			if (cast<InvokeInst>(value)->getCalledFunction()) {
				auto name = cast<InvokeInst>(value)->getCalledFunction()->getName();
				return name.startswith("llvm.dbg.") || name.startswith("llvm.lifetime.");
			}
		}
		if (isa<BranchInst>(value))
			return true;
		if (isa<GlobalVariable>(value)) {
			auto gv = cast<GlobalVariable>(value);
			if (gv->isConstant() && gv->getUnnamedAddr() == GlobalVariable::UnnamedAddr::Global)
				return true;
			if (gv->hasName() && gv->getName().startswith_lower("llvm."))
				return true;
		}
		return false;
	}

	char isMemorySource(Value *value) {
		if (isa<AllocaInst>(value) || isa<GlobalVariable>(value))
			return 1;
		if (isa<CallInst>(value) && cast<CallInst>(value)->getCalledFunction()) {
			auto name = cast<CallInst>(value)->getCalledFunction()->getName();
			if (name == "malloc" || name == "calloc")
				return 1;
			if (name == "posix_memalign")
				return 2;
		}
		return 0;
	}
}


// Pass that creates a PDG for all functions

char PDGCreationPass::ID = 0;

static RegisterPass<PDGCreationPass> reg("pdg-create", "PDG creator", false, true);

void PDGCreationPass::getAnalysisUsage(llvm::AnalysisUsage &Info) const {
	Info.setPreservesCFG();
	Info.setPreservesAll();
	Info.addRequired<UnifyFunctionExitNodes>();
}

bool PDGCreationPass::runOnModule(llvm::Module &M) {
	std::cout << "PDGCreationPass ..." << std::endl;
	std::cout << "PDG Node = " << sizeof(PDGNode) << "   PDG Edge = " << sizeof(PDGEdge) << std::endl;
	auto t = time(0);

	// Create trees for global variables and label them as memory source
	for (auto &global: M.getGlobalList()) {
		if (!isIgnoredInstruction(&global) && isMemorySource(&global)) {
			graph[graph.getVertex(&global)].source = true;
			graph[graph.getVertex(&global)].source_type = 1;
		}
	}

	int i = 1;
	for (auto &function: M.getFunctionList()) {
		dbg_cout << i++ << " / " << M.getFunctionList().size() << " functions (" << function.getName().str() << ") : "
				 << graph.num_vertices() << " vertices, " << graph.num_edges() << " edges, " << graph.capacity() << " capacity"
				 << std::endl;
		/*
		if (i > 1200000) {
			std::cout << time(0) - t << " seconds" << std::endl;
			std::ifstream f("/proc/self/status");
			std::string line;
			while (getline(f, line)) {
				if (line.substr(0, 1) == "V")
					std::cout << line << std::endl;
			}
			exit(0);
		}
		 */

		// Create function and argument vertices
		auto vFunc = graph.getVertex(&function);
		for (auto &arg: function.args()) {
			auto v = graph.getVertex(&arg, &function);
			graph.add_edge(vFunc, v, PDGEdge{.type=PDGEdge::Param, .index=arg.getArgNo()});
		}
		// Check if function is from another context
		if (externalSymbols.find(function.getName()) != externalSymbols.end())
			graph[vFunc].ipcfunction = true;

		for (auto &block: function.getBasicBlockList()) {
			for (auto &ins : block) {
				if (isIgnoredInstruction(&ins))
					continue;
				if (isa<ReturnInst>(ins)) {
					// Special case - function return
					if (cast<ReturnInst>(ins).getReturnValue() != nullptr) {
						graph.add_edge(vFunc, graph.getVertex(cast<ReturnInst>(ins).getReturnValue(), &function),
									   PDGEdge{.type=PDGEdge::Return, .index=0});
					}
				} else {
					// Default case - general instruction
					auto v = graph.getVertex(&ins, &function);
					graph[v].source_type = isMemorySource(&ins);
					graph[v].source = graph[v].source_type;
					addEdges(v, &ins);
				}
			}
		}
	}

	if (config->strongFDAnalysis) {
		analyzeStructs(M);
	}

#ifndef SILENT
	if (system("date 1>&2")) perror("system(date)");
#endif
	return false;
}

void PDGCreationPass::addEdges(Vertex v, User *instruction) {
	if (isa<LoadInst>(instruction)) {
		auto ptr = graph.getVertex(instruction->getOperand(0));
		//add_edge(getDeref(ptr), v, PDGEdge{.type = PDGEdge::Data}, graph);
		graph.addSubnodeEdges(graph.getDeref(ptr), v);
		if (isa<GEPOperator>(instruction->getOperand(0)) || isa<BitCastOperator>(instruction->getOperand(0))) {
			addEdges(ptr, cast<User>(instruction->getOperand(0)));
		}
		return;
	}
	if (isa<StoreInst>(instruction)) {
		// tmp for readability
		if (isIgnoredOperand(instruction->getOperand(0)))
			return;

		// llvm::outs() << "STORE   " << *instruction->getOperand(0) << " => " << *instruction->getOperand(1) << "\n";
		// llvm::outs() << "        " << *instruction->getOperand(0)->getType() << " => " << *instruction->getOperand(1)->getType() << "\n";
		auto val = graph.getVertex(instruction->getOperand(0));
		auto ptr = graph.getVertex(instruction->getOperand(1));
		// add_edge(val, graph.getDeref(ptr), PDGEdge{type: PDGEdge::Data}, graph);
		graph.addSubnodeEdges(val, graph.getDeref(ptr));
		if (isa<GEPOperator>(instruction->getOperand(1)) || isa<BitCastOperator>(instruction->getOperand(1))) {
			addEdges(ptr, cast<User>(instruction->getOperand(1)));
		}
		if ((!isa<GetElementPtrInst>(instruction->getOperand(0)) && isa<GEPOperator>(instruction->getOperand(0))) ||
			isa<BitCastOperator>(instruction->getOperand(0))) {
			addEdges(val, cast<User>(instruction->getOperand(0)));
		}
		return;
	}
	if (isa<CallInst>(instruction)) {
		auto *call = cast<CallInst>(instruction);
		if (isFileDescriptorFunction(getCalledFunction(call))) {
			dbg_llvm_outs << "[FD] Found fd construction: " << *call << "\n";
			graph[v].filedescriptor = true;
		}
		// bool isExternal = call->getCalledFunction() && externalSymbols.count(call->getCalledFunction()->getName()) > 0;
		for (auto &arg: call->arg_operands()) {
			// tmp for readability
			if (isIgnoredOperand(arg.get()))
				continue;
			auto v2 = graph.getVertex(arg.get());
			graph.add_edge(v2, v, PDGEdge{.type = PDGEdge::Param, .index=arg.getOperandNo()});
		}
		return;
	}
	if (isa<InvokeInst>(instruction)) {
		auto *call = cast<InvokeInst>(instruction);
		bool isExternal = call->getCalledFunction() && externalSymbols.count(call->getCalledFunction()->getName()) > 0;
		for (auto &arg: call->arg_operands()) {
			// tmp for readability
			if (isIgnoredOperand(arg.get()))
				continue;
			auto v2 = graph.getVertex(arg.get());
			graph.add_edge(v2, v, PDGEdge{.type = PDGEdge::Param, .index=arg.getOperandNo()});
		}
		return;
	}
	if (isa<GetElementPtrInst>(instruction) || isa<GEPOperator>(instruction)) {
		auto sourceVertex = graph.getVertex(instruction->getOperand(0));
		if (instruction->getNumOperands() < 3) {
			graph.addSubnodeEdges(sourceVertex, v);
		} else {
			graph.add_edge(sourceVertex, v, PDGEdge{.type= PDGEdge::Data, .index=0});
			auto sourceMemVertex = graph.getDerefOpt(sourceVertex);
			auto memV = graph.getDerefOpt(v);
			if (sourceMemVertex && memV) {
				specialGEPHandling(instruction, *sourceMemVertex);

				Vertex target = *sourceMemVertex;
				for (int i = 2; i < instruction->getNumOperands(); i++) {
					auto op = instruction->getOperand(i);
					if (isa<ConstantInt>(op)) {
						unsigned int index = cast<ConstantInt>(op)->getZExtValue();
						auto componentVertex = graph.getPartOpt(target, index);
						if (componentVertex) {
							// graph.uniteSubnodes(*componentVertex, *memV);
							target = *componentVertex;
						} else {
							// graph.uniteSubnodes(sourceVertex, v);
							break;
						}
					} else {
						// For example: fixed array access
						// %5 = getelementptr inbounds [129 x i32], [129 x i32]* @test, i64 0, i64 %4
						// graph.uniteSubnodes(sourceVertex, v);
						break;
					}
				}
				if (target != *sourceMemVertex) {
					graph.uniteSubnodes(target, *memV);
				} else {
					graph.uniteSubnodes(sourceVertex, v);
				}
			}
		}
		return;
	}
	if (isa<BitCastOperator>(instruction)) {
		auto sourceVertex = graph.getVertex(instruction->getOperand(0));
		graph.uniteSubnodes(sourceVertex, v);
	}
	/*
	if (isa<ReturnInst>(instruction)) {
		if (cast<ReturnInst>(instruction)->getReturnValue() != nullptr) {
			add_edge(getVertex(instruction->getFunction()), v, PDGEdge{.type=PDGEdge::Return}, graph);
		}
	}
	 */

	for (auto op = instruction->op_begin(); op != instruction->op_end(); op++) {
		if (!isa<BasicBlock>(op->get()) && !isa<Constant>(op->get())) {
			Vertex v2 = graph.getVertex(op->get());
			graph.addSubnodeEdges(v2, v);
			if (isa<GEPOperator>(op->get()) || isa<BitCastOperator>(op->get())) {
				addEdges(v2, cast<User>(op->get()));
			}
		}
	}
}

void PDGCreationPass::specialGEPHandling(llvm::User *ins, Vertex memVertex) {
	if (ins->getNumOperands() == 4 && isa<ConstantInt>(ins->getOperand(1)) && isa<ConstantInt>(ins->getOperand(2)) &&
		isa<ConstantInt>(ins->getOperand(3)) && cast<ConstantInt>(ins->getOperand(1))->isNullValue() &&
		cast<ConstantInt>(ins->getOperand(2))->isNullValue() && cast<ConstantInt>(ins->getOperand(3))->isNullValue()) {
		// c_str =
		// getelementptr inbounds %"class.std::__cxx11::basic_string", %"class.std::__cxx11::basic_string"* %0, i64 0, i32 0, i32 0
		// we want the second memory location grabbed
		auto pointedtype = ins->getOperand(0)->getType()->getPointerElementType();
		if (pointedtype && pointedtype->isStructTy()
			&& pointedtype->getStructName().startswith("class.std::") && pointedtype->getStructName().contains("::basic_string")) {
			auto some_union = graph.getPartOpt(memVertex, 2);
			if (some_union) {
				auto data_from_union = graph.getPartOpt(*some_union, 1);
				if (data_from_union) {
					// real memory: .0.0.deref
					auto v1 = graph.getPartOpt(memVertex, 0);
					if (v1) {
						auto v2 = graph.getPartOpt(*v1, 0);
						if (v2) {
							auto mem = graph.getDerefOpt(*v2);
							if (mem && *mem != *data_from_union) {
								graph.uniteSubnodes(*mem, *data_from_union);
							}
						}
					}
				}
			}
		}
	}
}


static bool struct_deep_equal(const StructType *st1, const StructType *st2, std::map<const StructType *, int> &equality) {
	if (st1->getStructNumElements() != st2->getStructNumElements())
		return false;
	for (unsigned int i = 0; i < st1->getStructNumElements(); i++) {
		auto t1 = st1->getStructElementType(i);
		auto t2 = st2->getStructElementType(i);
		if (t1 == t2)
			continue;
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

void PDGCreationPass::analyzeStructs(llvm::Module &M) {
	std::map<std::string, int> nameToCluster;
	std::vector<std::set<const StructType *>> clusters;
	for (const auto st: M.getIdentifiedStructTypes()) {
		auto str = st->getStructName();
		str = str.substr(0, str.find_last_not_of(".0123456789") + 1);
		auto it = nameToCluster.find(str);
		if (it != nameToCluster.end()) {
			graph.structClusters[st] = it->second;
			clusters[it->second].insert(st);
		} else {
			clusters.emplace_back();
			clusters.back().insert(st);
			graph.structClusters[st] = clusters.size() - 1;
		}
	}

	// Post-filter struct equality
	for (size_t i = 0; i < clusters.size(); i++) {
		size_t new_cluster_index = 0;
		const StructType *ref = nullptr;
		for (const auto st: clusters[i]) {
			if (ref) {
				if (!struct_deep_equal(st, ref, graph.structClusters)) {
					if (!new_cluster_index) {
						clusters.emplace_back();
						new_cluster_index = clusters.size() - 1;
					}
					clusters[new_cluster_index].insert(st);
				}
			} else {
				ref = st;
			}
		}
		if (new_cluster_index) {
			for (const auto st: clusters[new_cluster_index]) {
				graph.structClusters[st] = new_cluster_index;
			}
		}
	}
}
