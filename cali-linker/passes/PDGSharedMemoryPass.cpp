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
						if (ins.getParent() != &function.getEntryBlock()) {
							llvm::errs() << "[WARN] not implemented / potential memory leak: " << ins << "\n";
						} else {
							freeOnReturn.push_back(value);
						}
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
		for (auto ins: remove) {
			if (!ins->isSafeToRemove()) llvm::errs() << "NOT SAFE TO REMOVE: " << *ins << "\n";
			ins->eraseFromParent();
		}
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

	makeIndirectFunctionsShared(M);

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
	shm_realloc = M.getOrInsertFunction("realloc_to_shm", FunctionType::get(ptrtype, {ptrtype, int64type}, false));
	shm_free = M.getOrInsertFunction("shm_free", FunctionType::get(voidtype, {ptrtype}, false));
	shm_posix_memalign = M.getOrInsertFunction("posix_shm_memalign", FunctionType::get(int32type, {ptr2type, int64type, int64type}, false));
	shm_mmap = M.getOrInsertFunction("shm_mmap_share", FunctionType::get(ptrtype, {ptrtype, int64type, int32type, int32type, int32type, int64type}, false));
}

llvm::Constant* PDGSharedMemoryPass::getSharedMemoryReplacementFunc(Function* function, const std::string& name) {
	return function->getParent()->getOrInsertFunction(name, function->getFunctionType());
}

llvm::Constant *PDGSharedMemoryPass::getSharedMemoryFunction(llvm::Function *allocFunction) {
	std::string funcname = allocFunction->getName();
	if (StringRef(funcname).startswith("__ipc_specialized_"))
		funcname = funcname.substr(18);
	if (contextConfig) {
		auto it = contextConfig->functionBehavior.find(funcname);
		if (it != contextConfig->functionBehavior.end()) funcname = it->second;
	}

	if (funcname == "malloc") {
		return shm_malloc;
	} else if (funcname == "calloc") {
		return shm_calloc;
	} else if (funcname == "realloc") {
		return shm_realloc;
	} else if (funcname == "posix_memalign") {
		return shm_posix_memalign;
	} else if (funcname == "asprintf") {
		return getSharedMemoryReplacementFunc(allocFunction, "shm_asprintf");
	} else if (funcname == "vasprintf") {
		return getSharedMemoryReplacementFunc(allocFunction, "shm_vasprintf");
	} else if (funcname == "getenv") {
		return getSharedMemoryReplacementFunc(allocFunction, "shm_getenv");
	} else if (funcname == "strdup") {
		return getSharedMemoryReplacementFunc(allocFunction, "shm_strdup");
	} else if (funcname == "mmap" || funcname == "mmap64") {
		return shm_mmap;
	} else {
		return nullptr;
	}
}

void PDGSharedMemoryPass::makeAllocCallShared(llvm::CallInst &call) {
	auto f = getSharedMemoryFunction(call.getCalledFunction());
	if (f) {
		call.setCalledFunction(f);
	} else {
		errs() << "Warning: unsupported memory allocating function " << call.getCalledFunction()->getName() << "\n";
	}
}

llvm::Value *PDGSharedMemoryPass::makeStackAllocaShared(llvm::AllocaInst &ins) {
	if (ins.isArrayAllocation()) {
		// Dynamic-sized alloca
		llvm::errs() << "Make shared: " << ins << " in " << ins.getFunction()->getName() << "\n";
		llvm::IRBuilder<> builder(&ins);
		Value* sizeValue = ins.getOperand(0);
		size_t size = ins.getModule()->getDataLayout().getTypeAllocSize(ins.getType()->getPointerElementType());
		if (size != 1) {
			sizeValue = builder.CreateMul(sizeValue, ConstantInt::get(IntegerType::getInt64Ty(ins.getContext()), size));
		}
		if (ins.getAlignment() > 16) llvm::errs() << "[WARN] not implemented";
		llvm::Value *sharedSpace = builder.CreateCall(shm_malloc, {sizeValue}, ins.getName());
		sharedSpace = builder.CreatePointerCast(sharedSpace, ins.getType(), ins.getName());
		ins.replaceAllUsesWith(sharedSpace);
		return sharedSpace;
	}
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
				auto &vec = config->filedescriptors[function->getName()];
				if (std::find(vec.begin(), vec.end(), graph[e].index) == vec.end())
					vec.push_back(graph[e].index);
			}
		}
	}

	if (graph[*vOpt].filedescriptor) {
		dbg_llvm_outs << "Found FD return in function " << function->getName() << "!\n";
		auto &vec = config->filedescriptors[function->getName()];
		if (std::find(vec.begin(), vec.end(), -1) == vec.end())
			vec.push_back(-1);
	}
}

void PDGSharedMemoryPass::makeIndirectFunctionsShared(llvm::Module &M) {
	for (const auto& it: contextConfig->functionBehavior) {
		auto realfunc = M.getFunction(it.second);
		if (!realfunc) continue;
		auto replacement = getSharedMemoryFunction(realfunc);
		if (!replacement) continue;
		auto f = M.getFunction(it.first);
		if (!f) continue;

		std::vector<std::pair<User*, int>> uses;
		for (auto &use: f->uses()) {
			if (isa<CallInst>(use.getUser()) && cast<CallInst>(use.getUser())->getCalledFunction() == f) continue;
			if (isa<InvokeInst>(use.getUser()) && cast<InvokeInst>(use.getUser())->getCalledFunction() == f) continue;
			uses.emplace_back(use.getUser(), use.getOperandNo());
		}
		for (auto usepair: uses) {
			if (usepair.first->getOperand(usepair.second) == f) {
				usepair.first->setOperand(usepair.second, replacement);
				llvm::errs() << "Replacing " << it.first << " in " << *usepair.first << "\n";
			}
		}
	}
}
