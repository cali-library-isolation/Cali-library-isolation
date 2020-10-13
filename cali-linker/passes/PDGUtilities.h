#ifndef LLVMREWRITER_PDGUTILITIES_H
#define LLVMREWRITER_PDGUTILITIES_H

#include <cstddef>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Operator.h>
#include "PDG.h"
#include "../graph/color_map.h"

/**
 * Mark all nodes in these functions/SCC that can reach an IPC sink.
 * @param functions
 * @return number of initial IPC sinks this cluster contains. If == 0, then no sink is reachable.
 */
size_t calculateSinkReachability(PDG &graph, std::set<llvm::Function *> &functions, basegraph::ColorMap &colors);


/**
 * Mark all nodes in these function s/CSS that are reached by a file descriptor
 * @param graph
 * @param functions
 * @param colors
 * @return
 */
size_t calculateFDReachability(PDG &graph, std::set<llvm::Function *> &functions, basegraph::ColorMap &colors, bool useStructHeuristic);


inline bool isIgnoredOperand(llvm::Value *value) {
	return !value || llvm::isa<llvm::ConstantData>(value)/*|| llvm::isa<llvm::ConstantPointerNull>(value)*/;
}

template<class InstTy>
inline llvm::Function *getCalledFunction(llvm::CallBase<InstTy> &call) {
	auto f = call.getCalledFunction();
	if (f) return f;
	auto op = call.getCalledValue();
	// llvm::outs() << "[TMP] Operand #0: " << *op << "\n";
	if (op && llvm::isa<llvm::BitCastOperator>(op)) {
		auto op2 = llvm::cast<llvm::BitCastOperator>(op)->getOperand(0);
		if (op2 && llvm::isa<llvm::Function>(op2)) {
			// llvm::outs() << "[TMP] Extracted deep call target: " << call << "   =>   " << llvm::cast<llvm::Function>(op2)->getName() << "\n";
			return llvm::cast<llvm::Function>(op2);
		}
	}
	return nullptr;
}

template<class InstTy>
inline llvm::Function *getCalledFunction(llvm::CallBase<InstTy> *call) {
	return getCalledFunction(*call);
}

bool isFileDescriptorFunction(llvm::Function *function);

int isFileDescriptorParamFunction(llvm::Function *function);

bool isFileDescriptorPtrFunction(llvm::Function *function);

inline bool isFileDescriptorType(llvm::Type *type) {
	return type && type->isIntegerTy() && type->getIntegerBitWidth() >= 16;
}

template<class InstTy>
inline std::vector<InstTy *> getCallsToFunction(llvm::User *function) {
	std::vector<InstTy *> result;
	for (auto &use: function->uses()) {
		if (llvm::isa<InstTy>(use.getUser())) {
			result.push_back(llvm::cast<InstTy>(use.getUser()));
		} else if (llvm::isa<llvm::BitCastOperator>(use.getUser())) {
			for (auto ins: getCallsToFunction<InstTy>(use.getUser())) {
				result.push_back(ins);
			}
		}
	}
	return result;
}

void markAsPointerToShared(PDG& graph, Vertex v);
void markAsShared(PDG& graph, Vertex v);

#endif //LLVMREWRITER_PDGUTILITIES_H
