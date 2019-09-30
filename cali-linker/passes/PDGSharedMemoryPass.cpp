#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>
#include "PDGSharedMemoryPass.h"
#include "PDGCreationPass.h"
#include "../cali_linker/debug.h"
#include "PDGUtilities.h"

using namespace llvm;

char PDGSharedMemoryPass::ID = 0;

static RegisterPass<PDGSharedMemoryPass> shmReg("pdg-sharedmemory", "PDG Shared Memory Allocation", false, false);

void PDGSharedMemoryPass::getAnalysisUsage(llvm::AnalysisUsage &Info) const {
	Info.addRequired<PDGCreationPass>();
}

bool PDGSharedMemoryPass::runOnModule(llvm::Module &M) {
	std::cout << "--- PDGSharedMemoryPass ---" << std::endl;
	addSharedMemoryFunctions(M);
	pdg = &getAnalysis<PDGCreationPass>().graph;

	for (auto &function: M.getFunctionList()) {
		std::vector<Value *> freeOnReturn;
		std::vector<Instruction *> returns;
		std::vector<Instruction *> remove;

		for (auto &block: function.getBasicBlockList()) {
			for (auto &ins : block) {
				if (isa<CallInst>(ins)) {
					//if (func && isa<Function>(func) && func->getName() == "malloc") {
					if (shouldBeShared(&ins)) {
						auto func = cast<CallInst>(ins).getCalledFunction();
						dbg_llvm_outs << "    (call in function \"" << ins.getFunction()->getName() << "\")\n";
						makeAllocCallShared(cast<CallInst>(ins));
					}
					checkForFileDescriptorParameters(cast<CallInst>(ins));
				} else if (isa<AllocaInst>(ins)) {
					if (shouldBeShared(&ins)) {
						dbg_llvm_outs << "    (variable in function \"" << ins.getFunction()->getName() << "\")\n";
						auto &alloc = cast<AllocaInst>(ins);
						auto value = makeStackAllocaShared(alloc);
						freeOnReturn.push_back(value);
						remove.push_back(&ins);
					}
				} else if (isa<ReturnInst>(ins)) {
					returns.push_back(&ins);
				}
			}
		}

		if (!freeOnReturn.empty()) {
			for (auto ret: returns) {
				IRBuilder<> builder(ret);
				for (auto op: freeOnReturn) {
					auto op2 = builder.CreatePointerCast(op, Type::getInt8PtrTy(M.getContext()));
					builder.CreateCall(shm_free, {op2});
				}
			}
		}
		for (auto ins: remove)
			ins->removeFromParent();
	}

	std::vector<GlobalVariable *> globals;
	for (auto &global: M.getGlobalList()) {
		if (isa<GlobalVariable>(global) && !global.isConstant() && !global.isDeclaration()) {
			// outs() << "Global: " << global << " | " << *global.getBaseObject() << "\n";
			//if (global.getName().startswith("shared_"))
			if (shouldBeShared(&global)) {
				dbg_llvm_outs << "    (variable \"" << global.getName() << "\")\n";
				globals.push_back(&cast<GlobalVariable>(global));
			}
		}
	}
	makeGlobalVariablesShared(M, globals);

	return true;
}

bool PDGSharedMemoryPass::shouldBeShared(llvm::Value *def) {
	auto &graph = *pdg;

	auto it = graph.value_to_vertex.find(def);
	if (it == graph.value_to_vertex.end())
		return false;
	if (!graph[it->second].source)
		return false;

	auto vMemory = graph.getMemoryForSource(it->second);
	if (!vMemory)
		return false;
	if (graph[*vMemory].ipcsink) {
		dbg_llvm_outs << "Sharing: " << *def << " : immediate use\n";
		return true;
	}

	// Check reachability
	if (graph[*vMemory].reaches_ipc_sink) {
		dbg_llvm_outs << "Sharing: " << *def << " : reaches sink\n";
		return true;
	}

	return false;
}

void PDGSharedMemoryPass::addSharedMemoryFunctions(llvm::Module &M) {
	auto ptrtype = Type::getInt8PtrTy(M.getContext());
	auto ptr2type = PointerType::get(ptrtype, 0);
	auto int64type = Type::getInt64Ty(M.getContext());
	auto int32type = Type::getInt32Ty(M.getContext());
	auto voidtype = Type::getVoidTy(M.getContext());
	shm_malloc = M.getOrInsertFunction("shm_malloc", FunctionType::get(ptrtype, {int64type}, false));
	shm_calloc = M.getOrInsertFunction("shm_calloc", FunctionType::get(ptrtype, {int64type, int64type}, false));
	shm_free = M.getOrInsertFunction("shm_free", FunctionType::get(voidtype, {ptrtype}, false));
	shm_posix_memalign = M.getOrInsertFunction("posix_shm_memalign", FunctionType::get(int32type, {ptr2type, int64type, int64type}, false));
}

void PDGSharedMemoryPass::makeAllocCallShared(llvm::CallInst &call) {
	auto funcname = call.getCalledFunction()->getName();
	if (funcname == "malloc") {
		call.setCalledFunction(shm_malloc);
	} else if (funcname == "calloc") {
		call.setCalledFunction(shm_calloc);
	} else if (funcname == "posix_memalign") {
		call.setCalledFunction(shm_posix_memalign);
	} else {
		errs() << "Warning: unsupported memory allocating function " << funcname << "\n";
	}
}

llvm::Value *PDGSharedMemoryPass::makeStackAllocaShared(llvm::AllocaInst &ins) {
	size_t size = ins.getModule()->getDataLayout().getTypeAllocSize(ins.getType()->getPointerElementType());
	if (ins.getAlignment() > 16)
		size += ins.getAlignment();
	llvm::IRBuilder<> builder(&ins);
	if (size == 0) {
		errs() << "Warning: Allocating variable with size 0! " << ins << "\n";
	}
	llvm::Value *sharedSpace = builder.CreateCall(shm_malloc, {ConstantInt::get(IntegerType::getInt64Ty(ins.getContext()), size)}, ins.getName());
	sharedSpace = builder.CreatePointerCast(sharedSpace, ins.getType(), ins.getName());
	ins.replaceAllUsesWith(sharedSpace);
	return sharedSpace;
}

void PDGSharedMemoryPass::makeGlobalVariablesShared(llvm::Module &M, std::vector<llvm::GlobalVariable *> &vars) {
	/*auto startSymbol = dyn_cast<GlobalVariable>(M.getOrInsertGlobal("__shm_data_begin_"+to_string(rand()), Type::getInt1Ty(M.getContext())));
	startSymbol->setSection("shm_data");
	startSymbol->setAlignment(4096);
	startSymbol->setInitializer(ConstantInt::get(Type::getInt1Ty(M.getContext()), 0));*/
	//startSymbol->setLinkage(GlobalVariable::InternalLinkage);

	if (vars.empty()) {
		auto v = cast<GlobalVariable>(M.getOrInsertGlobal("__stop_shm_data", Type::getInt8Ty(M.getContext())));
		v->setInitializer(ConstantInt::get(v->getType()->getPointerElementType(), 0));
		vars.push_back(v);
	}

	bool align = true;
	for (auto gv: vars) {
		dbg_llvm_outs << "Sharing global " << *gv << "\n";
		gv->setSection("shm_data");
		if (align) {
			gv->setAlignment(4096);
			align = false;
		}
	}

	auto v = cast<GlobalVariable>(M.getOrInsertGlobal("__last_shm_data_variable", Type::getInt8Ty(M.getContext())));
	v->setInitializer(ConstantInt::get(v->getType()->getPointerElementType(), 0));
	v->setSection("shm_data");
	v->setAlignment(4096);
}

void PDGSharedMemoryPass::checkForFileDescriptorParameters(llvm::CallInst &call) {
	auto &graph = *pdg;
	auto vOpt = graph.getVertexOpt(&call);
	if (!vOpt) return;
	auto function = getCalledFunction(call);
	if (!function) return;
	// check for export functions
	if (!function->hasName() || externalSymbols.find(function->getName()) == externalSymbols.end())
		return;

	for (const auto &e: graph.in_edges(*vOpt)) {
		if (graph[e].type == PDGEdge::Param && graph[e].index < call.getNumArgOperands()) {
			if (graph[e.source].filedescriptor) {
				dbg_llvm_outs << "[FD] found fd param: #" << graph[e].index << " in " << function->getName() << "\n";
				config->filedescriptors[function->getName()] = graph[e].index;
			}
		}
	}
}
