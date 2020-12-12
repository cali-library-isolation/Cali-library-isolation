
#include "llvm_module.h"
#include "../passes/PDGReachabilityPass.h"
#include "../passes/PDGCreationPass.h"
#include "../passes/PDGWriterPass.h"
#include "../passes/PDGSharedMemoryPass.h"
#include "../passes/PDGInstrumentPass.h"
#include "../passes/PDGSpecializationPass.h"
#include "../passes/DataDrivenSCCPass.h"
#include "../passes/FunctionAliasResolverPass.h"
#include "../cali_linker/debug.h"
#include "../passes/PDGSharedReachabilityPass.h"
#include "../passes/SimpleSharedAnalysisPass.h"
#include <iostream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Analysis/MemoryDependenceAnalysis.h>
#include <llvm/IR/Verifier.h>
#include <llvm/CodeGen/DIE.h>
#include <llvm/CodeGen/UnreachableBlockElim.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/LinkAllPasses.h>
#include <fstream>

namespace ipcrewriter {

	using namespace std;
	using namespace llvm;
	using namespace llvm::legacy;

	llvm::LLVMContext LlvmIpcModule::context;

	LlvmIpcModule::LlvmIpcModule(const std::string &source, bool mainModule, YamlConfig *config, const ContextConfig *contextConfig)
			: IpcModule(source, mainModule, config, contextConfig) {}

	void LlvmIpcModule::loadModuleFile() {
		if (!module) {
			SMDiagnostic error;
			module = std::move(parseIRFile(source, error, context));
			if (!module) throw runtime_error("Could not read/parse IR file");
		}

		// find imports / exports
		for (const auto &function : module->functions()) {
			if (function.isDeclaration()) {
				imports.insert(function.getGlobalIdentifier());
			} else {
				exports.insert(function.getGlobalIdentifier());
			}
		}
	}

	void LlvmIpcModule::rewriteFunctionCalls(CommunicationPair &cmp, const std::set<std::string> &symbols) {
		preventInlining();
		runAnalysisTodo(symbols);
		if (contextConfig->instrument_coverage) {
			logEntries.emplace_back("=== COVERAGE ===");
		}

		for (auto &function : module->functions()) {
			if (!function.isDeclaration()) continue;
			StringRef name = function.getName();
			if (symbols.find(name) != symbols.end()) {
				// Function *replacement = cmp.getOrCreateWrapperFunction(function, this, &callInst);
				while (function.use_begin() != function.use_end()) {
					auto &use = *function.use_begin();
					if (isa<CallInst>(use.getUser()) && cast<CallInst>(use.getUser())->getCalledFunction() == &function) {
						dbg_llvm_outs << "Replacing in call " << *use.getUser() << "\n";
						Function *replacement = cmp.getOrCreateWrapperFunction(&function, this, cast<CallInst>(use.getUser()));
						dbg_llvm_outs << "   " << *cast<CallInst>(use.getUser())->getCalledFunction()->getType() << "   =>   " << *replacement->getType()
									  << "\n";
						if (contextConfig->instrument_coverage) {
							std::string desc = std::string("call to ") + function.getName().str() + " from " +
											   cast<Instruction>(use.getUser())->getFunction()->getName().str();
							replacement = instrumentCoverageReplacement(replacement, desc);
						}
						// use.set(replacement);
						cast<CallInst>(use.getUser())->setCalledFunction(replacement);
					} else if (isa<InvokeInst>(use.getUser()) && cast<InvokeInst>(use.getUser())->getCalledFunction() == &function) {
						dbg_llvm_outs << "Replacing in invoke " << *use.getUser() << "\n";
						Function *replacement = cmp.getOrCreateWrapperFunction(&function, this, cast<InvokeInst>(use.getUser()));
						dbg_llvm_outs << "   " << *cast<InvokeInst>(use.getUser())->getCalledFunction()->getType() << "   =>   " << *replacement->getType()
									  << "\n";
						if (contextConfig->instrument_coverage) {
							auto desc = "invoke to " + function.getName().str() + " from " + cast<Instruction>(use.getUser())->getFunction()->getName().str();
							replacement = instrumentCoverageReplacement(replacement, desc);
						}
						// use.set(replacement);
						cast<InvokeInst>(use.getUser())->setCalledFunction(replacement);
					} else {
						dbg_llvm_outs << "Replacing in ???? " << *use.getUser() << "\n";
						Function *replacement = cmp.getOrCreateWrapperFunction<CallInst>(&function, this, nullptr);
						if (contextConfig->instrument_coverage) {
							auto f = dyn_cast<Instruction>(use.getUser());
							auto desc = "call to " + function.getName().str() + " taken reference in " +
										(f && f->getFunction() ? f->getFunction()->getName().str() : "?");
							replacement = instrumentCoverageReplacement(replacement, desc);
						}
						use.set(replacement);
						dbg_llvm_outs << "              =>  " << *use.getUser() << "\n";
						break;
					}
				}
			}
			/*
			for (auto &bb : function) {
				for (auto ins = bb.begin(), insend = bb.end(); ins != insend; ins++) {
					if (isa<CallInst>(ins)) {
						auto &callInst = cast<CallInst>(*ins);
						auto calledFunction = callInst.getCalledFunction();
						if (calledFunction) {
							StringRef name = calledFunction->getName();
							if (symbols.find(name) != symbols.end()) {
								// this is a call to the other module!
								Function *replacement = cmp.getOrCreateWrapperFunction(calledFunction, this, &callInst);
								// errs() << "replacing " << name << " with " << replacement->getName() << " in " << *ins << "\n";
								callInst.setCalledFunction(replacement);
							}
						}
					}
				}
			}
			*/
		}

		if (contextConfig->instrument_coverage) {
			auto coverageFunction = module->getFunction("__coverage_count");
			if (coverageFunction) {
				std::vector<User*> users(coverageFunction->users().begin(), coverageFunction->users().end());
				for (auto user: users) {
					auto call = dyn_cast<CallInst>(user);
					call->setArgOperand(1, ConstantInt::get(Type::getInt32Ty(context), next_coverage_id));
				}
			}
		}

		writeConfiguration();

		for (const auto& it: contextConfig->functionBehavior) {
			if (it.second == "free") {
				handleFreeFunction(it.first);
			}
		}

		auto vp = createVerifierPass();
		vp->doInitialization(*module);
		for (auto &f: module->functions())
			vp->runOnFunction(f);
		vp->doFinalization(*module);
	}

	void LlvmIpcModule::save() {
		error_code ec;
		raw_fd_ostream os(rewrittenFilename()[0], ec, llvm::sys::fs::F_None);
		WriteBitcodeToFile(*module, os);
		assert(!ec);
	}

	std::vector<std::string> LlvmIpcModule::rewrittenFilename() {
		return {source.substr(0, source.length() - 3) + "-ipc.bc"};
	}

	void LlvmIpcModule::runAnalysisTodo(const set<string> &symbols) {
		//TODO
		usleep(10000);
		auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
		cout << "--- Passes running now ---" << endl;
		// outs() << *module << "\n";

		legacy::PassManager pm;
		pm.add(createVerifierPass(true));
		pm.add(createPromoteMemoryToRegisterPass());
		pm.add(createAggressiveDCEPass());
		pm.add(createGlobalDCEPass());
		pm.add(createCFGSimplificationPass());
		pm.add(createUnifyFunctionExitNodesPass());

		// pm.add(new MemoryDependenceWrapperPass());

		pm.add(new FunctionAliasResolverPass());
		pm.add(new DataDrivenSCCPass(config, contextConfig));
		pm.add(new PDGCreationPass(config, contextConfig, symbols));
		pm.add(new PDGReachabilityPass(config));
		pm.add(new PDGSpecializationPass(config));
		// TODO if?
		//pm.add(new DataDrivenSCCPass(config, contextConfig));
		//pm.add(new PDGSharedReachabilityPass(config, contextConfig));
		if (contextConfig->warnings) {
			pm.add(new SimpleSharedAnalysisPass(config));
		}
		if (config && config->writeGraphs) {
			auto pass = new PDGWriterPass();
			pass->runDot = config->writeGraphsDot;
			pm.add(pass);
		}
		if (contextConfig && contextConfig->instrument) {
			cout << "Instrumentation scheduled..." << endl;
			pm.add(new PDGInstrumentPass(ignored));
		}
		pm.add(new PDGSharedMemoryPass(config, contextConfig, symbols));
		pm.add(createVerifierPass(true));
		// pm.add(new CallGraphWrapperPass());
		// pm.add(new OldSCCAnalysis());
		pm.run(*module);

		llvm::outs().flush();
		auto dt = std::chrono::high_resolution_clock::now().time_since_epoch() - t;
		usleep(10000);
		cout << "\n--- Passes finished (in " << std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() << " ms) ---" << endl;
	}

	llvm::Function *LlvmIpcModule::instrumentCoverageReplacement(llvm::Function *function, const std::string description) {
		std::cerr << "Coverage " << next_coverage_id << ": " << description << std::endl;
		logEntries.push_back("Coverage " + std::to_string(next_coverage_id) + ": " + description);
		// Create function that replaces this handler
		auto newFunction = cast<Function>(module->getOrInsertFunction("__coverage_" + std::to_string(next_coverage_id), function->getFunctionType()));
		newFunction->setCallingConv(function->getCallingConv());
		newFunction->addFnAttr(Attribute::NoInline);
		auto bb = BasicBlock::Create(module->getContext(), "entry", newFunction);
		IRBuilder<> builder(bb);
		// Start with call to __coverage_count
		auto Int32 = Type::getInt32Ty(context);
		auto count = module->getOrInsertFunction("__coverage_count", FunctionType::get(Type::getVoidTy(context), {Int32, Int32}, false));
		builder.CreateCall(count, {ConstantInt::get(Int32, next_coverage_id), ConstantInt::get(Int32, 0)});
		// Follow with "regular" call
		std::vector<Value *> params;
		for (auto &a: newFunction->args()) params.push_back(&a);
		auto ret = builder.CreateCall(function, params);
		if (function->getReturnType()->isVoidTy()) {
			builder.CreateRetVoid();
		} else {
			builder.CreateRet(ret);
		}
		next_coverage_id++;
		return newFunction;
	}

	void LlvmIpcModule::writeConfiguration() {
		/*
		 * Struct format:
		 * int is_forking;
		 * int silent;
		 * int concurrentLibraryCommunication
		 */
		auto Int32 = IntegerType::getInt32Ty(context);
		auto type = StructType::get(context, {Int32, Int32, Int32}, false);
		auto value = ConstantStruct::get(type, {
			ConstantInt::get(Int32, contextConfig->programIsForking ? 1 : 0),
			ConstantInt::get(Int32, contextConfig->silent),
			ConstantInt::get(Int32, contextConfig->concurrentLibraryCommunication ? 1 : 0),
		});
		new GlobalVariable(*module, type, true, GlobalValue::ExternalLinkage, value, "program_params");
	}

	llvm::Value *generateConstantString(const std::string &data, IRBuilder<> &builder) {
		auto init = ConstantDataArray::getString(builder.getContext(), data, true);
		auto var = new GlobalVariable(*builder.GetInsertBlock()->getModule(), init->getType(), true, GlobalVariable::PrivateLinkage, init, ".str");
		return builder.CreateBitCast(var, Type::getInt8PtrTy(builder.getContext()));
	}


	void LlvmIpcModule::handleFreeFunction(const std::string& name) {
		auto f = module->getFunction(name);
		if (!f) {
			llvm::errs() << "[WARN] Function " << name << " not found!\n";
			return;
		}

		auto Void = Type::getVoidTy(context);
		auto VoidPtr = Type::getInt8PtrTy(context);
		auto free_wrapper = module->getOrInsertFunction("shm_free_wrapper", FunctionType::get(Void, {VoidPtr, VoidPtr}, false));

		if (f->isDeclaration()) {
			auto Int64 = Type::getInt64Ty(context);
			auto pointerType = f->getFunctionType()->getPointerTo();
			auto dlsym = module->getOrInsertFunction("dlsym", FunctionType::get(VoidPtr, {VoidPtr, VoidPtr}, false));

			auto var = new GlobalVariable(*module, pointerType, false, GlobalValue::InternalLinkage, ConstantPointerNull::get(pointerType), "__real_" + f->getName());
			auto bb1 = BasicBlock::Create(context, "entry", f);
			auto bb2 = BasicBlock::Create(context, "need_dlsym", f);
			auto bb3 = BasicBlock::Create(context, "call", f);
			IRBuilder<> builder(bb1);
			auto ptr = builder.CreateLoad(var);
			builder.CreateCondBr(builder.CreateICmpNE(ptr, ConstantPointerNull::get(pointerType)), bb3, bb2);
			builder.SetInsertPoint(bb2);
			auto RTLD_NEXT = builder.CreateIntToPtr(ConstantInt::get(Int64, -1l, true), VoidPtr);
			auto llvm_string = generateConstantString(name, builder);
			auto ptr2 = builder.CreateBitCast(builder.CreateCall(dlsym, {RTLD_NEXT, llvm_string}), pointerType);
			builder.CreateStore(ptr2, var);
			builder.CreateBr(bb3);
			builder.SetInsertPoint(bb3);
			auto ptrPhi = builder.CreatePHI(pointerType, 2);
			ptrPhi->addIncoming(ptr, bb1);
			ptrPhi->addIncoming(ptr2, bb2);
			builder.CreateCall(free_wrapper, {builder.CreateBitCast(f->arg_begin(), VoidPtr), builder.CreateBitCast(ptrPhi, VoidPtr)});
			if (f->getReturnType()->isVoidTy()) {
				builder.CreateRetVoid();
			} else {
				builder.CreateRet(ConstantData::getNullValue(f->getReturnType()));
			}

		} else {
			f->setName(name + "_original");
			auto replacement = cast<Function>(module->getOrInsertFunction(name, f->getFunctionType(), f->getAttributes()));
			replacement->setCallingConv(f->getCallingConv());
			f->replaceAllUsesWith(replacement);

			auto bb = BasicBlock::Create(context, "entry", replacement);
			IRBuilder<> builder(bb);
			builder.CreateCall(free_wrapper, {builder.CreateBitCast(replacement->arg_begin(), VoidPtr), builder.CreateBitCast(f, VoidPtr)});
			if (f->getReturnType()->isVoidTy()) {
				builder.CreateRetVoid();
			} else {
				builder.CreateRet(ConstantData::getNullValue(f->getReturnType()));
			}
		}
	}

	void LlvmIpcModule::preventInlining() {
		for (const auto &it: contextConfig->functionBehavior) {
			auto f = module->getFunction(it.first);
			if (f && !f->isDeclaration()) {
				f->addFnAttr(Attribute::NoInline);
			}
		}
	}

}