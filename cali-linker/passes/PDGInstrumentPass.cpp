#include <iostream>
#include "PDGInstrumentPass.h"
#include "PDGCreationPass.h"
#include "PDGReachabilityPass.h"
#include "PDGSpecializationPass.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

#define FULL_INSTRUMENTATION


char PDGInstrumentPass::ID = 0;

// static RegisterPass<PDGInstrumentPass> reg("pdg-instrument", "PDG Instrumentation for statistics", false, false);

void PDGInstrumentPass::getAnalysisUsage(AnalysisUsage &Info) const {
	Info.addRequired<PDGCreationPass>();
	Info.addPreserved<PDGCreationPass>();
	Info.addPreserved<PDGReachabilityPass>();
	Info.addPreserved<PDGSpecializationPass>();
}

llvm::ConstantInt *PDGInstrumentPass::getInt64(llvm::Module &M, uint64_t value) {
	return ConstantInt::get(Type::getInt64Ty(M.getContext()), value);
}

void PDGInstrumentPass::prepareStubs(llvm::Module &M) {
	Type *tVoid = Type::getVoidTy(M.getContext());
	Type *tInt64 = Type::getInt64Ty(M.getContext());
	// void insInit();
	M.getOrInsertFunction("insInit", FunctionType::get(tVoid, {}, false));
	// void insAddMemoryNode(uint64_t id, uint64_t type, uint64_t is_shared, char *def, char *function);
	M.getOrInsertFunction("insAddMemoryNode", FunctionType::get(tVoid, {tInt64, tInt64, tInt64}, true));
	// void insAlloc(uint64_t id, void *start, uint64_t size);
	M.getOrInsertFunction("insAlloc", FunctionType::get(tVoid, {tInt64}, true));
	// void insFree(void *addr);
	M.getOrInsertFunction("insFree", FunctionType::get(tVoid, {}, true));
}

bool PDGInstrumentPass::runOnModule(Module &M) {
	std::cout << "PDGInstrumentPass ..." << std::endl;
	prepareStubs(M);
	PDG &pdg = getAnalysis<PDGCreationPass>().graph;

	Function *init = (Function *) M.getOrInsertFunction("__instrumentation_init", FunctionType::get(Type::getVoidTy(M.getContext()), false));
	BasicBlock::Create(M.getContext(), "entry", init);
	IRBuilder<> builder(&init->getEntryBlock());
	builder.CreateCall(M.getFunction("insInit"));

	long node_count = 0;
	long node_shared_count = 0;

	// Init - post list of all memory nodes
	for (auto entry: pdg.value_to_vertex) {
		Vertex v = entry.second;
		PDGNode &n = pdg[v];

		if (n.source && (!n.function || ignored.find(n.function->getName()) == ignored.end())) {
			uint64_t type = 0;
			if (isa<AllocaInst>(n.instruction)) type = 1;
			else if (isa<GlobalVariable>(n.instruction)) type = 2;
			else if (isa<CallInst>(n.instruction)) type = 3;

			StringRef functionName = "";
			if (isa<Instruction>(n.instruction)) {
				auto ins = cast<Instruction>(n.instruction);
				if (ins->getFunction() && ins->getFunction()->hasName()) {
					functionName = ins->getFunction()->getName();
				}
			}

			std::string insText;
			llvm::raw_string_ostream rso(insText);
			rso << *n.instruction;
			rso.flush();
			if (insText.length() > 1000) {
				insText = insText.substr(0, 800) + "  [...]  " + insText.substr(insText.length() - 80, 80);
			}

			auto vMemory = pdg.getMemoryForSource(v);
			bool isShared = vMemory && (pdg[*vMemory].ipcsink || pdg[*vMemory].reaches_ipc_sink);

			builder.CreateCall(M.getFunction("insAddMemoryNode"), {
					getInt64(M, v), getInt64(M, type), getInt64(M, isShared ? 1 : 0),
					builder.CreateGlobalStringPtr(insText), builder.CreateGlobalStringPtr(functionName)
			});
			node_count++;
			if (isShared) node_shared_count++;
			if (isShared) llvm::outs() << "SHARED: " << functionName << " | " << insText << "\n";

			// Alloc-signal for all global variables
			if (isa<GlobalVariable>(n.instruction)) {
				uint64_t size = M.getDataLayout().getTypeAllocSize(n.instruction->getType()->getPointerElementType());
				builder.CreateCall(M.getFunction("insAlloc"), {getInt64(M, v), n.instruction, getInt64(M, size)});
			} else {
#ifdef FULL_INSTRUMENTATION
				instrumentAllocation(M, pdg, v, n.instruction);
#endif
			}
		}
	}

	llvm::outs() << "[Instrument] nodes = " << node_count << " | shared = " << node_shared_count << "\n";

	// Instrument all alloca/malloc occurences

	// Instrument end-of-scope for all alloca occurences

	builder.CreateRetVoid();

	return true;
}

void PDGInstrumentPass::instrumentAllocation(Module &M, PDG &pdg, uint64_t id, Value *instruction) {
	if (isa<AllocaInst>(instruction)) {
		auto ins = cast<AllocaInst>(instruction);
		IRBuilder<> builder(ins->getNextNode());
		builder.CreateCall(M.getFunction("insAlloc"), {getInt64(M, id), instruction, getInt64(M, M.getDataLayout().getTypeAllocSize(ins->getAllocatedType()))});

		// deallocate. inefficient, but works
		if (ins->getParent() == &ins->getFunction()->getEntryBlock()) {
			for (auto i: getReturns(ins->getFunction())) {
				builder.SetInsertPoint(i);
				builder.CreateCall(M.getFunction("insFree"), {ins});
			}
		} else {
			llvm::errs() << "Warning: alloca not in entry block: " << *ins << "\n";
		}

	} else if (isa<CallInst>(instruction)) {
		auto ins = cast<CallInst>(instruction);
		auto name = ins->getCalledFunction()->getName();
		IRBuilder<> builder(ins->getNextNode());

		Value *size;
		if (name == "malloc") {
			size = ins->getArgOperand(0);
		} else if (name == "calloc") {
			size = builder.CreateMul(ins->getArgOperand(0), ins->getArgOperand(1));
		} else {
			errs() << "Unsupported memory-allocating function: " << name << "\n";
			return;
		}

		builder.CreateCall(M.getFunction("insAlloc"), {getInt64(M, id), instruction, size});

	} else {
		errs() << "Unsupported instrumentation: " << *instruction << "\n";
	}
}

std::vector<llvm::ReturnInst *> &PDGInstrumentPass::getReturns(llvm::Function *function) {
	auto it = functionReturns.find(function);
	if (it != functionReturns.end())
		return it->second;

	for (auto &bb: *function) {
		for (auto &i: bb) {
			if (isa<ReturnInst>(i)) {
				functionReturns[function].push_back(cast<ReturnInst>(&i));
			}
		}
	}
	return functionReturns[function];
}

