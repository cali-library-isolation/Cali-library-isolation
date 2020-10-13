#include "communication_pair.h"
#include <iostream>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InlineAsm.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/TypeBuilder.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/Verifier.h>
#include "../cali_linker/debug.h"
#include "../cali_linker/linker_replacement.h"
#include "llvm_module.h"


namespace ipcrewriter {

	using namespace std;
	using namespace llvm;


	int CommunicationPair::next_id = 1;

	CommunicationPair::CommunicationPair(IpcModule *module1, IpcModule *module2, bool concurrentLibraryCommunication)
			: id(next_id++), next_code(3), module1(module1), module2(module2), concurrentLibraryCommunication(concurrentLibraryCommunication) {
		logEntries.emplace_back("=== COMMUNICATION ===");
		// initModule(LlvmIpcModule::context);
	}

	CommunicationPair::~CommunicationPair() {
		for (auto &it: wrappedFunctions) {
			if (it.second) {
				// printf("- %d : %s\n", it.second->code, it.first.c_str());
				delete it.second;
			}
		}
	}


	int CommunicationPair::getId() const {
		return id;
	}

	void CommunicationPair::initModule(LLVMContext &context) {
		// load basic module
		filesystem::path filename = applicationPath;
		filename.append("ipc-communication-proto.bc");
		SMDiagnostic error;
		module = parseIRFile(filename.string(), error, context);
		if (!module) throw runtime_error("Could not read/parse IR file");
		module->setSourceFileName("ipc-communication-" + to_string(id) + ".cpp");

		// generate basic debug infos
		diBuilder.reset(new DIBuilder(*module));
		diUnit = diBuilder->createFile("ipc-communication-" + to_string(id) + ".cpp", "ipc-modularizer");
		diBuilder->createCompileUnit(dwarf::DW_LANG_C99, diUnit, "IPC Modularizer", false, "", 0);
		module->addModuleFlag(Module::Warning, "Debug Info Version", DEBUG_METADATA_VERSION);

		// retrieve references
		// {function code, callback id, data start}
		basicCommMemType = module->getTypeByName("struct.__ipc_data");
		assert(basicCommMemType);
		processIpcCall1.initialize(module->getFunction("process_ipc_call_1"), *this);
		processIpcCall2.initialize(module->getFunction("process_ipc_call_2"), *this);
		wrapOutgoingCallback = module->getFunction("wrap_outgoing_callback");
		wrapIncomingCallback = module->getFunction("wrap_incoming_callback");
		wrapOutgoingVaList = module->getFunction("wrap_outgoing_valist");
		wrapPointerFromCallback = module->getFunction("wrap_pointer_from_callback");
		unwrapPointerFromCallback = module->getFunction("unwrap_pointer_from_callback");
		ipcShareFD = module->getFunction("ipc_share_fd");
		ipcLockSending = module->getFunction("ipc_lock_sending");
		nsjailConfigure = module->getFunction("nsjail_configure");
		// module->getFunction("__unused_stuff_for_nsjail_configure")->removeFromParent();
		module->getFunction("__unused_stuff_for_nsjail_configure")->setLinkage(InternalFunctionLinkage);

		// retrieve shm functions
		i8PtrType = IntegerType::getInt8PtrTy(context, 0);
		auto sizeType = IntegerType::getInt64Ty(context);
		auto intType = IntegerType::getInt32Ty(context);
		auto boolType = IntegerType::getInt1Ty(context);
		shm_malloc = cast<Function>(module->getOrInsertFunction("shm_malloc", FunctionType::get(i8PtrType, {sizeType}, false)));
		memcpy = cast<Function>(module->getOrInsertFunction("llvm.memcpy.p0i8.p0i8.i64",
															FunctionType::get(Type::getVoidTy(context), {i8PtrType, i8PtrType, sizeType, boolType}, false)));
		free = cast<Function>(module->getOrInsertFunction("free", FunctionType::get(Type::getVoidTy(context), {i8PtrType}, false)));

		// repair linkage
		module->getFunction("trigger_ipc_call")->setLinkage(InternalFunctionLinkage);
		module->getFunction("ipc_wait_for_return")->setLinkage(InternalFunctionLinkage);
		module->getFunction("ipc_share_fd")->setLinkage(InternalFunctionLinkage);
		module->getFunction("process_ipc_call_1")->setLinkage(InternalFunctionLinkage);
		module->getFunction("process_ipc_call_2")->setLinkage(InternalFunctionLinkage);
		module->getFunction("wrap_outgoing_callback")->setLinkage(InternalFunctionLinkage);
		module->getFunction("wrap_pointer_from_callback")->setLinkage(InternalFunctionLinkage);

		// create libipc-internal replacements
		createInternalReplacements();
	}

	template<class InstTy>
	Function *CommunicationPair::getOrCreateWrapperFunction(Function *callee, IpcModule *sendingModule, InstTy *callInst) {
		LLVMContext &context = callee->getContext();
		if (!module) initModule(context);


		// determine the name the wrapped function is going to have
		string wrappedName;
		if (!callee->isVarArg() || callInst == nullptr || callee->getFunctionType()->getFunctionNumParams() == callInst->getNumArgOperands()) {
			wrappedName = "__ipc_replacement_" + to_string(id) + "_" + callee->getGlobalIdentifier();
		} else {
			wrappedName = "__ipc_replacement_" + to_string(id) + "_" + callee->getGlobalIdentifier() + "_vararg_";
			std::string type_str;
			llvm::raw_string_ostream rso(type_str);
			for (unsigned int i = 0; i < callInst->getNumArgOperands(); i++) {
				callInst->getArgOperand(i)->getType()->print(rso);
				rso << ", ";
			}
			size_t hash = std::hash<string>()(rso.str());
			wrappedName += to_string(hash);
		}

		auto wrapped = wrappedFunctions.find(wrappedName);
		WrapperFunctionInfos *wrapper;
		if (wrapped == wrappedFunctions.end()) {
			wrapper = createReplacementFunction(wrappedName, callee, callInst);
			if (sendingModule != module1)
				createReplacementHandler(*wrapper, processIpcCall1, 0);
			if (sendingModule != module2)
				createReplacementHandler(*wrapper, processIpcCall2, 1);
		} else {
			wrappedName = wrapped->second->wrapper->getName();
			wrapper = wrapped->second;
		}

		// add declaration if necessary
		Constant *decl = callee->getParent()->getOrInsertFunction(wrappedName, wrapper->wrapper->getFunctionType(), callee->getAttributes());
		assert(isa<Function>(*decl));
		return &cast<Function>(*decl);
	}

	// Instantiate template functions
	template Function *CommunicationPair::getOrCreateWrapperFunction(Function *callee, IpcModule *sendingModule, CallInst *callInst);

	template Function *CommunicationPair::getOrCreateWrapperFunction(Function *callee, IpcModule *sendingModule, InvokeInst *callInst);


	template<class InstTy>
	WrapperFunctionInfos *CommunicationPair::createReplacementFunction(string &replacementName, llvm::Function *originalFunction, InstTy *callInst) {
		LLVMContext &context = originalFunction->getContext();
		Constant *getSendingStruct = module->getOrInsertFunction("ipc_get_sending_struct", FunctionType::get(PointerType::get(basicCommMemType, 0), false));
		IntegerType *Int = Type::getInt32Ty(context);

		FunctionType *functionType = originalFunction->getFunctionType();
		if (functionType->isVarArg()) {
			vector<Type *> argumentTypes(callInst->getNumArgOperands());
			// first add "real" argument types
			for (unsigned int i = 0; i < functionType->getFunctionNumParams(); i++) {
				argumentTypes[i] = functionType->getFunctionParamType(i);
			}
			// add vararg types from call
			for (unsigned int i = functionType->getFunctionNumParams(); i < callInst->getNumArgOperands(); i++) {
				argumentTypes[i] = callInst->getArgOperand(i)->getType();
			}

			functionType = FunctionType::get(functionType->getReturnType(), argumentTypes, false);
		}
		Function *replacement = Function::Create(functionType, GlobalValue::LinkageTypes::ExternalLinkage, replacementName, &*module);
		IRBuilder<> builder(BasicBlock::Create(context, "entry", replacement));

		// User instrumentation
		auto userInstrumentName = "instrument_before_"+originalFunction->getName().str();
		if (userInstrumentedFunctions.find(userInstrumentName) != userInstrumentedFunctions.end()) {
			auto userInstrumentFunction = (Function*) module->getOrInsertFunction(userInstrumentName, replacement->getFunctionType());
			std::vector<Value*> args;
			for (auto &arg: replacement->args()) args.push_back(&arg);
			builder.CreateCall(userInstrumentFunction, args);
		}

		//TODO debug
		// auto timing_debug = module->getOrInsertFunction("__ipc_timing_debug_simple", FunctionType::get(Type::getVoidTy(context), {IntegerType::getInt8PtrTy(context)}, false));
		// builder.CreateCall(timing_debug, {generateNonConstantString(":ipc call: " + originalFunction->getName().str(), builder)});

		// handle FD arguments
		std::map<int, Value*> fdValues;
		if (fileDescriptorPolicy && !fileDescriptorPolicy->empty()) {
			auto it = fileDescriptorPolicy->find(originalFunction->getName());
			if (it != fileDescriptorPolicy->end()) {
				for (int fdIndex: it->second) {
					if (fdIndex < 0) continue;
					auto arg = replacement->arg_begin();
					std::advance(arg, fdIndex);
					if (!arg->getType()->isIntegerTy()) {
						llvm::errs() << "WARN INVALID FD TYPE: " << *arg->getType() << " " << *arg << "\n";
						continue;
					}
					if (arg->getType()->getIntegerBitWidth() == 32) {
						fdValues[fdIndex] = builder.CreateCall(ipcShareFD, {arg});
					} else {
						auto Int32 = Type::getInt32Ty(context);
						fdValues[fdIndex] = builder.CreateIntCast(builder.CreateCall(ipcShareFD, {builder.CreateIntCast(arg, Int32, true)}), arg->getType(), true);
					}
				}
			}
		}

		// write parameters to shm
		if (concurrentLibraryCommunication) {
			builder.CreateCall(ipcLockSending);
		}
		Instruction *sendingData = builder.CreateCall(getSendingStruct);
		ArrayRef<Type *> paramTypes(functionType->param_begin(), functionType->param_end());
		vector<Value *> paramValues;
		if (!paramTypes.empty()) {
			StructType *paramStruct = StructType::create(paramTypes);
			PointerType *paramStructPtr = PointerType::get(paramStruct, 0);

			Value *paramPtr = builder.CreateStructGEP(basicCommMemType, sendingData, basicCommMemTypeIdxData);
			paramPtr = builder.CreateBitCast(paramPtr, paramStructPtr);
			unsigned int i = 0;
			for (auto &arg: replacement->args()) {
				Value *value = wrapOutgoingValue(&arg, builder);
				// check for fd
				auto fdValue = fdValues.find(i);
				if (fdValue != fdValues.end()) {
					dbg_llvm_outs << " -> Wrapped outgoing fd #" << i << ": " << *value << "\n";
					logEntries.push_back(" -> Wrapped outgoing fd #" + std::to_string(i));
					value = fdValue->second;
				}
				paramValues.push_back(value);
				i++;
			}

			for (i = 0; i < paramValues.size(); i++) {
				builder.CreateStore(paramValues[i], builder.CreateStructGEP(paramStruct, paramPtr, i));
			}
		}

		// write function code to shm
		unsigned int code = next_code++;
		builder.CreateStore(ConstantInt::get(Int, code), builder.CreateStructGEP(basicCommMemType, sendingData, 0));
		logEntries.push_back("Code " + std::to_string(code) + ": function " + originalFunction->getName().str());

		// wait for IPC result
		builder.CreateCall(module->getOrInsertFunction("trigger_ipc_call", FunctionType::get(Type::getVoidTy(context), false)));
		Value *result = builder.CreateCall(module->getOrInsertFunction("ipc_wait_for_return", FunctionType::get(PointerType::get(basicCommMemType, 0), false)));

		// TODO debug
		// builder.CreateCall(timing_debug, {generateNonConstantString(":ipc ret: " + originalFunction->getName().str(), builder)});

		// Cleanup for "wrapOutgoingValue"
		if (!paramTypes.empty()) {
			int i = 0;
			for (auto &arg: replacement->args()) {
				freeWrapOutgoingValueResult(&arg, paramValues[i], builder);
				i++;
			}
		}

		// User instrumentation
		userInstrumentName = "instrument_after_"+originalFunction->getName().str();
		if (!functionType->getReturnType()->isVoidTy()) {
			// load result from shm
			result = builder.CreateStructGEP(basicCommMemType, result, basicCommMemTypeIdxData);
			result = builder.CreateBitCast(result, PointerType::get(functionType->getReturnType(), 0));
			result = builder.CreateLoad(result);
			result = wrapIncomingValue(result, builder);
			if (userInstrumentedFunctions.find(userInstrumentName) != userInstrumentedFunctions.end()) {
				auto userInstrumentFunction = (Function *) module->getOrInsertFunction(
						userInstrumentName,
						FunctionType::get(Type::getVoidTy(context), {result->getType()}, false)
				);
				builder.CreateCall(userInstrumentFunction, {result});
			}
			builder.CreateRet(result);
		} else {
			if (userInstrumentedFunctions.find(userInstrumentName) != userInstrumentedFunctions.end()) {
				auto userInstrumentFunction = (Function*) module->getOrInsertFunction(userInstrumentName, FunctionType::get(Type::getVoidTy(context), {}, false));
				builder.CreateCall(userInstrumentFunction, {});
			}
			builder.CreateRetVoid();
		}

		for (const auto &it: fdValues) {
			logEntries.push_back("  with FD argument " + to_string(it.first));
		}

		//auto it = wrappedFunctions.emplace(replacementName, WrapperFunctionInfos(originalFunction, replacement, code));
		//return it.first->second;
		wrappedFunctions[replacementName] = new WrapperFunctionInfos(originalFunction, replacement, code);
		return wrappedFunctions[replacementName];
	}

	static Metadata *getDIType(DIBuilder *diBuilder, llvm::Type *type) {
		if (type->isVoidTy()) return diBuilder->createUnspecifiedType("void");
		if (type->isIntegerTy())
			return diBuilder->createBasicType("int" + to_string(type->getIntegerBitWidth()), type->getIntegerBitWidth(), dwarf::DW_AT_base_types);
		if (type->isPointerTy()) return diBuilder->createPointerType(diBuilder->createUnspecifiedType("void"), type->getPrimitiveSizeInBits());
		return diBuilder->createUnspecifiedType("<unknownType>");
	}

	void CommunicationPair::createReplacementHandler(WrapperFunctionInfos &wfi, IpcCallHandlerFunction &hf, int processIpcCallId) {
		// already wrapped?
		if (wfi.inProcessIpcCall[processIpcCallId]) return;

		// Add case to switch
		LLVMContext &context = wfi.wrapper->getContext();
		IntegerType *Int = Type::getInt32Ty(context);
		BasicBlock *handler = BasicBlock::Create(context, "handle_" + to_string(wfi.code) + "_" + wfi.wrapper->getName(), hf.function);
		IRBuilder<> builder(handler);
		hf.switchInst->addCase(ConstantInt::get(Int, wfi.code), handler);
		Value *receivingData = &*hf.function->args().begin();

		builder.SetCurrentDebugLocation(DebugLoc::get(10000 * wfi.code + 0, 0, hf.debug));

		// declare function in comm module
		Value *func = module->getOrInsertFunction(wfi.original->getName(), wfi.wrapper->getFunctionType());
		/*if (isa<Function>(func)){
			Function* f = cast<Function>(func);
			if (f->getSubprogram() == nullptr){
				FunctionType* ft = wfi.wrapper->getFunctionType();
				vector<Metadata*> types;
				// result type
				types.push_back(getDIType(diBuilder.get(), ft->getReturnType()));
				// arguments
				for (auto& arg : ft->params()){
					types.push_back(getDIType(diBuilder.get(), arg));
				}
				auto diFunctionType = diBuilder->createSubroutineType(diBuilder->getOrCreateTypeArray(types));
				f->setSubprogram(diBuilder->createFunction(diUnit, f->getName(), "", diUnit, 0, diFunctionType, true, true, 0));
			}
		}*/

		// read parameters from shm
		ArrayRef<Type *> paramTypes(wfi.wrapper->getFunctionType()->param_begin(), wfi.wrapper->getFunctionType()->param_end());
		vector<Value *> params;
		if (!paramTypes.empty()) {
			StructType *paramStruct = StructType::create(paramTypes);
			PointerType *paramStructPtr = PointerType::get(paramStruct, 0);

			Value *paramPtr = builder.CreateStructGEP(basicCommMemType, receivingData, basicCommMemTypeIdxData);
			paramPtr = builder.CreateBitCast(paramPtr, paramStructPtr);
			unsigned int i = 0;
			for (auto &arg: wfi.wrapper->args()) {
				builder.SetCurrentDebugLocation(DebugLoc::get(10000 * wfi.code + 1 + i, 0, hf.debug));
				Value *argument = builder.CreateLoad(builder.CreateStructGEP(paramStruct, paramPtr, i++));
				params.push_back(wrapIncomingValue(argument, builder));
			}
		}

		// Call original function
		builder.SetCurrentDebugLocation(DebugLoc::get(10000 * wfi.code + 100, 0, hf.debug));
		Value *result = builder.CreateCall(func, params);

		// Wrap result if it is a file descriptor
		if (fileDescriptorPolicy && !fileDescriptorPolicy->empty()) {
			auto it = fileDescriptorPolicy->find(func->getName());
			if (it != fileDescriptorPolicy->end() && std::find(it->second.begin(), it->second.end(), -1) != it->second.end()) {
				logEntries.push_back(" -> wrap result as FD");
				result = builder.CreateCall(ipcShareFD, {result});
			}
		}

		// Save result to shm
		if (concurrentLibraryCommunication && (wfi.code <= 2 || wfi.code > 8)) {  // "internal" functions must not be interrupted
			builder.CreateCall(ipcLockSending, {});
		}
		builder.SetCurrentDebugLocation(DebugLoc::get(10000 * wfi.code + 200, 0, hf.debug));
		if (!wfi.wrapper->getFunctionType()->getReturnType()->isVoidTy()) {
			Value *resultMemory = builder.CreateStructGEP(basicCommMemType, hf.sendingStruct, basicCommMemTypeIdxData);
			resultMemory = builder.CreateBitCast(resultMemory, PointerType::get(wfi.wrapper->getFunctionType()->getReturnType(), 0));
			result = wrapOutgoingValue(result, builder);
			builder.CreateStore(result, resultMemory);
			/*
			if (result->getType()->isPointerTy()) { // debug code
				builder.CreateCall(module->getFunction("debug_i64"), ConstantInt::get(Type::getInt64Ty(context), wfi.code, false));
				builder.CreateCall(module->getFunction("debug_i64"), builder.CreatePtrToInt(result, Type::getInt64Ty(context)));
				builder.CreateCall(module->getFunction("debug_i64"), builder.CreatePtrToInt(resultMemory, Type::getInt64Ty(context)));
			} else if (result->getType()->isIntegerTy()) {
				builder.CreateCall(module->getFunction("debug_i64"), ConstantInt::get(Type::getInt64Ty(context), wfi.code, false));
				builder.CreateCall(module->getFunction("debug_i64"), builder.CreateIntCast(result, Type::getInt64Ty(context), false));
				builder.CreateCall(module->getFunction("debug_i64"), builder.CreatePtrToInt(resultMemory, Type::getInt64Ty(context)));
			}
			// */
		}

		builder.CreateBr(hf.switchEpilog);

		wfi.inProcessIpcCall[processIpcCallId] = true;
	}

	void CommunicationPair::save(const std::string &filename) {
		diBuilder->finalize();
		if (llvm::verifyModule(*module, &llvm::errs()))
			throw std::runtime_error("Communication module broken");
		error_code ec;
		raw_fd_ostream os(filename, ec, llvm::sys::fs::F_None);
		WriteBitcodeToFile(*module, os);
		assert(!ec);
		cerr << "Saved " << filename << endl;
	}

	void CommunicationPair::createInternalReplacements() {
		LLVMContext &context = module->getContext();
		Type *voidty = Type::getVoidTy(context);
		Type *voidptr = Type::getInt8PtrTy(context);
		IntegerType *sizet = IntegerType::getIntNTy(context, 8 * (sizeof(size_t)));
		IntegerType *Int = Type::getInt32Ty(context);

		// void *internal_mremap(void *addr, size_t old_length, size_t length, int flags);
		/*vector<Type *> params({voidptr, sizet, sizet, Int});
		Function *func = cast<Function>(module->getOrInsertFunction("internal_mremap", FunctionType::get(voidptr, params, false)));
		string name = "__ipc_replacement_internal_mremap";
		auto newWrapper = createReplacementFunction<CallInst>(name, func, nullptr);
		createReplacementHandler(*newWrapper, processIpcCall1, 0);
		createReplacementHandler(*newWrapper, processIpcCall2, 1);*/

		// void internal_remap_shm(void);
		vector<Type *> params({});
		Function *func = cast<Function>(module->getOrInsertFunction("internal_remap_shm", FunctionType::get(voidty, params, false)));
		string name = "__ipc_replacement_internal_remap_shm";
		auto newWrapper = createReplacementFunction<CallInst>(name, func, nullptr);
		createReplacementHandler(*newWrapper, processIpcCall1, 0);
		createReplacementHandler(*newWrapper, processIpcCall2, 1);

		// void *internal_mmap(void *addr, size_t old_length, size_t length, int flags);
		params = {voidptr, sizet, Int, Int, Int, sizet};
		func = cast<Function>(module->getOrInsertFunction("internal_mmap", FunctionType::get(voidptr, params, false)));
		name = "__ipc_replacement_internal_mmap";
		newWrapper = createReplacementFunction<CallInst>(name, func, nullptr);
		createReplacementHandler(*newWrapper, processIpcCall1, 0);
		createReplacementHandler(*newWrapper, processIpcCall2, 1);

		// void internal_setSharedFDSize();
		func = cast<Function>(module->getOrInsertFunction("internal_setSharedFDSize", FunctionType::get(Type::getVoidTy(context), {}, false)));
		name = "__ipc_replacement_internal_setSharedFDSize";
		newWrapper = createReplacementFunction<CallInst>(name, func, nullptr);
		createReplacementHandler(*newWrapper, processIpcCall1, 0);
		createReplacementHandler(*newWrapper, processIpcCall2, 1);

		// int internal_receive_fd(void* fileDescriptorMapping, int fd);
		func = cast<Function>(module->getOrInsertFunction("internal_receive_fd", FunctionType::get(Int, {voidptr, Int, Int}, false)));
		name = "__ipc_replacement_internal_receive_fd";
		newWrapper = createReplacementFunction<CallInst>(name, func, nullptr);
		createReplacementHandler(*newWrapper, processIpcCall1, 0);
		createReplacementHandler(*newWrapper, processIpcCall2, 1);

		// void internal_close_fd(int local_fd);
		func = cast<Function>(module->getOrInsertFunction("internal_close_fd", FunctionType::get(Type::getVoidTy(context), {Int, Int}, false)));
		name = "__ipc_replacement_internal_close_fd";
		newWrapper = createReplacementFunction<CallInst>(name, func, nullptr);
		createReplacementHandler(*newWrapper, processIpcCall1, 0);
		createReplacementHandler(*newWrapper, processIpcCall2, 1);

		// sighandler_t internal_signal(int signum, sighandler_t handler);
		// typedef void (*sighandler_t)(int);
		auto SighandlerT = FunctionType::get(Type::getVoidTy(context), {Int}, false)->getPointerTo();
		func = cast<Function>(module->getOrInsertFunction("internal_signal", FunctionType::get(SighandlerT, {Int, SighandlerT}, false)));
		name = "__ipc_replacement_internal_signal";
		newWrapper = createReplacementFunction<CallInst>(name, func, nullptr);
		createReplacementHandler(*newWrapper, processIpcCall1, 0);
		createReplacementHandler(*newWrapper, processIpcCall2, 1);
	}

	llvm::Value *CommunicationPair::wrapOutgoingValue(llvm::Value *value, llvm::IRBuilder<> &builder) {
		// Check for callbacks
		if (value->getType()->isPointerTy() && value->getType()->getPointerElementType()->isFunctionTy()) {
			LLVMContext &context = module->getContext();
			Type *voidptr = Type::getInt8PtrTy(context);
			auto wrapped = builder.CreateCall(wrapOutgoingCallback, {builder.CreateBitCast(value, voidptr), getCallbackTypeHandler(value->getType())});
			value = builder.CreateBitCast(wrapped, value->getType());
			dbg_llvm_outs << "-> Wrapping outgoing callback " << *value->getType() << " in " << builder.GetInsertBlock()->getParent()->getName() << "\n";
		} else if (value->getType()->isPointerTy() && value->getType()->getPointerElementType()->isStructTy() &&
				   value->getType()->getPointerElementType()->getStructName() == "struct.__va_list_tag") {
			// Wrap va_list instances that get passed through the IPC bridge
			Type *voidptr = Type::getInt8PtrTy(module->getContext());
			auto value_void = builder.CreateCall(wrapOutgoingVaList, {builder.CreateBitCast(value, voidptr)});
			value = builder.CreateBitCast(value_void, value->getType());
			dbg_llvm_outs << "-> Cloning va_list " << *value << "\n";
		}
		return value;
	}

	void CommunicationPair::freeWrapOutgoingValueResult(llvm::Value *argument, llvm::Value *wrapped, llvm::IRBuilder<> &builder) {
		if (argument->getType()->isPointerTy() && argument->getType()->getPointerElementType()->isStructTy() &&
			argument->getType()->getPointerElementType()->getStructName() == "struct.__va_list_tag") {
			// builder.CreateCall(free, {builder.CreateBitCast(wrapped, i8PtrType)});
		}
	}

	llvm::Value *CommunicationPair::wrapIncomingValue(llvm::Value *value, llvm::IRBuilder<> &builder) {
		// Check for callbacks
		if (value->getType()->isPointerTy() && value->getType()->getPointerElementType()->isFunctionTy()) {
			LLVMContext &context = module->getContext();
			Type *voidptr = Type::getInt8PtrTy(context);
			Function *replacement = getCallbackTypeReplacement(value->getType());
			auto unwrapped = builder.CreateCall(wrapIncomingCallback, {builder.CreateBitCast(value, voidptr), builder.CreateBitCast(replacement, voidptr)});
			value = builder.CreateBitCast(unwrapped, value->getType());
			dbg_llvm_outs << "-> Wrapping incoming callback " << *value->getType() << "\n";
			std::string buffer;
			llvm::raw_string_ostream os(buffer);
			value->print(os);
			logEntries.push_back("  with callback valueid=" + os.str());
		}
		return value;
	}

	llvm::Function *CommunicationPair::getCallbackTypeHandler(llvm::Type *type) {
		auto lookup = callbackTypeHandlers.find(type);
		if (lookup != callbackTypeHandlers.end()) return lookup->second;

		LLVMContext &context = module->getContext();
		Type *voidptr = Type::getInt8PtrTy(context);
		IntegerType *Int = Type::getInt32Ty(context);
		Type *basicCommMemTypePtr = PointerType::get(basicCommMemType, 0);
		Type *Void = Type::getVoidTy(context);
		FunctionType *handlerFunctionType = FunctionType::get(Void, {voidptr, voidptr, voidptr}, false);
		// handler: (callback-ptr, receiving data, sending data)
		Function *function = Function::Create(handlerFunctionType, InternalFunctionLinkage, "__ipc_callback_handler_" + to_string((uintptr_t) type), &*module);
		IRBuilder<> builder(BasicBlock::Create(context, "entry", function));

		auto it = function->args().begin();
		Value *callback = builder.CreateBitCast(&*it, type);
		it++;
		Value *receivingData = builder.CreateBitCast(&*it, basicCommMemTypePtr);
		it++;
		Value *sendingData = builder.CreateBitCast(&*it, basicCommMemTypePtr);

		// read parameters from shm
		FunctionType *functionType = cast<FunctionType>(type->getPointerElementType());
		ArrayRef<Type *> paramTypes(functionType->param_begin(), functionType->param_end());
		vector<Value *> params;
		if (paramTypes.size() > 0) {
			StructType *paramStruct = StructType::create(paramTypes);
			PointerType *paramStructPtr = PointerType::get(paramStruct, 0);

			Value *paramPtr = builder.CreateStructGEP(basicCommMemType, receivingData, basicCommMemTypeIdxData);
			paramPtr = builder.CreateBitCast(paramPtr, paramStructPtr);
			for (unsigned int i = 0; i < functionType->getNumParams(); i++) {
				Value *argument = builder.CreateLoad(builder.CreateStructGEP(paramStruct, paramPtr, i));
				params.push_back(wrapIncomingValue(argument, builder));
			}
		}
		// Call original function
		Value *result = builder.CreateCall(callback, params);
		// Save result to shm
		if (concurrentLibraryCommunication) {
			builder.CreateCall(ipcLockSending);
		}
		if (!functionType->getReturnType()->isVoidTy()) {
			Value *resultMemory = builder.CreateStructGEP(basicCommMemType, sendingData, basicCommMemTypeIdxData);
			resultMemory = builder.CreateBitCast(resultMemory, PointerType::get(functionType->getReturnType(), 0));
			result = wrapOutgoingValue(result, builder);
			builder.CreateStore(result, resultMemory);
		}
		// Save result code (1 = return)
		builder.CreateStore(ConstantInt::get(Int, 1), builder.CreateStructGEP(basicCommMemType, sendingData, 0));

		builder.CreateRetVoid();

		callbackTypeHandlers[type] = function;
		return function;
	}

	/**
	 * Generate a function with signature matching type. Additional input is register r11 (= callback index).
	 * This function can be called by a callback trampoline (which sets r11 and then delegates here).
	 * @param type
	 * @return LLVM function that can be used to replace a callback pointer.
	 */
	llvm::Function *CommunicationPair::getCallbackTypeReplacement(llvm::Type *type) {
		auto lookup = callbackTypeReplacements.find(type);
		if (lookup != callbackTypeReplacements.end()) return lookup->second;

		LLVMContext &context = module->getContext();
		Constant *getSendingStruct = module->getOrInsertFunction("ipc_get_sending_struct", FunctionType::get(PointerType::get(basicCommMemType, 0), false));
		IntegerType *Int = Type::getInt32Ty(context);
		Type *voidptr = Type::getInt8PtrTy(context);
		FunctionType *functionType = cast<FunctionType>(type->getPointerElementType());

		Function *replacement = Function::Create(functionType, InternalFunctionLinkage, "__ipc_callback_replacement_" + to_string((uintptr_t) type), &*module);
		IRBuilder<> builder(BasicBlock::Create(context, "entry", replacement));
		Value *callbackGetter = InlineAsm::get(FunctionType::get(voidptr, false), "movq %r11, $0", "=r", true);
		Value *callback = builder.CreateCall(callbackGetter);
		Instruction *sendingData = builder.CreateCall(getSendingStruct);

		auto diType = diBuilder->createSubroutineType(diBuilder->getOrCreateTypeArray(vector<Metadata *>()));
		DISubprogram *diSub = diBuilder->createFunction(diUnit, replacement->getName(), StringRef(), diUnit, 0, diType, true, true, 0);
		replacement->setSubprogram(diSub);

		// write parameters to shm
		ArrayRef<Type *> paramTypes(functionType->param_begin(), functionType->param_end());
		std::vector<Value *> copiedValues;
		std::vector<Value *> arguments;
		if (!paramTypes.empty()) {
			for (auto &arg: replacement->args()) {
				arguments.push_back(wrapOutgoingValue(&arg, builder));
			}
		}
		if (concurrentLibraryCommunication) {
			builder.CreateCall(ipcLockSending);
		}
		std::vector<std::pair<Value*, Value*>> modifiedPointers;
		if (!paramTypes.empty()) {
			StructType *paramStruct = StructType::create(paramTypes);
			PointerType *paramStructPtr = PointerType::get(paramStruct, 0);
			Value *paramPtr = builder.CreateStructGEP(basicCommMemType, sendingData, basicCommMemTypeIdxData);
			paramPtr = builder.CreateBitCast(paramPtr, paramStructPtr);
			unsigned int i = 0;
			for (auto value: arguments) {
				auto addr = builder.CreateStructGEP(paramStruct, paramPtr, i++);
				if (value->getType()->isPointerTy()) {
					// we check if that's a pointer to something on the stack. If so, we have to copy.
					auto tmpOld = builder.CreateBitCast(value, wrapPointerFromCallback->getReturnType());
					auto tmpNew = builder.CreateCall(wrapPointerFromCallback, {tmpOld});
					value = builder.CreateBitCast(tmpNew, value->getType());
					modifiedPointers.emplace_back(tmpOld, tmpNew);
				}
				builder.CreateStore(value, addr);
			}
		}

		// Write callback number to shm
		builder.CreateStore(callback, builder.CreateStructGEP(basicCommMemType, sendingData, 2));

		// write function code to shm => 2 = callback
		builder.CreateStore(ConstantInt::get(Int, 2), builder.CreateStructGEP(basicCommMemType, sendingData, 0));

		// wait for IPC result
		builder.CreateCall(module->getOrInsertFunction("trigger_ipc_call", FunctionType::get(Type::getVoidTy(context), false)));
		Value *result = builder.CreateCall(module->getOrInsertFunction("ipc_wait_for_return", FunctionType::get(PointerType::get(basicCommMemType, 0), false)));
		for (const auto& it: modifiedPointers) {
			std::vector<Value*> unwrapArguments;
			unwrapArguments.push_back(it.first);
			unwrapArguments.push_back(it.second);
			unwrapArguments.push_back(ConstantInt::get(Type::getInt32Ty(context), modifiedPointers.size() - 1));
			for (const auto& it2: modifiedPointers) {
				if (it2.first != it.first) unwrapArguments.push_back(it2.first);
			}
			builder.CreateCall(unwrapPointerFromCallback, unwrapArguments);
		}
		if (!functionType->getReturnType()->isVoidTy()) {
			// load result from shm
			result = builder.CreateStructGEP(basicCommMemType, result, basicCommMemTypeIdxData);
			result = builder.CreateBitCast(result, PointerType::get(functionType->getReturnType(), 0));
			result = builder.CreateLoad(result);
			result = wrapIncomingValue(result, builder);
			builder.CreateRet(result);
		} else {
			builder.CreateRetVoid();
		}

		callbackTypeReplacements.insert({type, replacement});
		return replacement;
	}

	void CommunicationPair::setNsjailConfig(const PermissionConfig *permissions) {
		auto Int = IntegerType::getInt32Ty(module->getContext());
		auto Int64 = IntegerType::getInt64Ty(module->getContext());
		auto block = nsjailConfigure->getBasicBlockList().begin();
		Instruction *ins = &*block->begin();
		IRBuilder<> builder(ins);
		auto conf = &*nsjailConfigure->arg_begin();

		// Mounts
		for (const auto &path: permissions->readonly) {
			builder.CreateCall(module->getFunction("nsjail_addMountRO"), {conf, generateNonConstantString(path, builder)});
		}
		for (const auto &path: permissions->readwrite) {
			builder.CreateCall(module->getFunction("nsjail_addMountRW"), {conf, generateNonConstantString(path, builder)});
		}
		for (const auto &path: permissions->tmpfs) {
			builder.CreateCall(module->getFunction("nsjail_addMountTmp"), {conf, generateNonConstantString(path, builder)});
		}

		// User/Network
		if (!permissions->user.empty()) {
			builder.CreateCall(module->getFunction("nsjail_setUID"), {conf, generateNonConstantString(permissions->user, builder)});
		}
		if (!permissions->group.empty()) {
			builder.CreateCall(module->getFunction("nsjail_setGID"), {conf, generateNonConstantString(permissions->group, builder)});
		}
		if (!permissions->network.empty()) {
			builder.CreateCall(module->getFunction("nsjail_setNetwork"), {conf, generateNonConstantString(permissions->network, builder)});
		}

		// Filters/seccomp
		if (!permissions->seccomp.empty()) {
			builder.CreateCall(module->getFunction("nsjail_setSeccomp"), {conf, generateNonConstantString(permissions->seccomp, builder)});
		}
		for (const auto &cap: permissions->capabilities) {
			builder.CreateCall(module->getFunction("nsjail_addCapability"), {conf, generateNonConstantString(cap, builder)});
		}
		if (permissions->max_cpus > 0) {
			builder.CreateCall(module->getFunction("nsjail_setMaxCPU"), {conf, ConstantInt::get(Int, permissions->max_cpus)});
		}
		if (permissions->silent) {
			builder.CreateCall(module->getFunction("nsjail_setSilent"), {conf});
		}

		// Rlimit
		if (!permissions->rlimit.as.empty()) {
			builder.CreateCall(module->getFunction("nsjail_set_rlimit_as"), {conf, generateNonConstantString(permissions->rlimit.as, builder)});
		}
		if (!permissions->rlimit.core.empty()) {
			builder.CreateCall(module->getFunction("nsjail_set_rlimit_core"), {conf, generateNonConstantString(permissions->rlimit.core, builder)});
		}
		if (!permissions->rlimit.cpu.empty()) {
			builder.CreateCall(module->getFunction("nsjail_set_rlimit_cpu"), {conf, generateNonConstantString(permissions->rlimit.cpu, builder)});
		}
		if (!permissions->rlimit.fsize.empty()) {
			builder.CreateCall(module->getFunction("nsjail_set_rlimit_fsize"), {conf, generateNonConstantString(permissions->rlimit.fsize, builder)});
		}
		if (!permissions->rlimit.nofile.empty()) {
			builder.CreateCall(module->getFunction("nsjail_set_rlimit_nofile"), {conf, generateNonConstantString(permissions->rlimit.nofile, builder)});
		}
		if (!permissions->rlimit.nproc.empty()) {
			builder.CreateCall(module->getFunction("nsjail_set_rlimit_nproc"), {conf, generateNonConstantString(permissions->rlimit.nproc, builder)});
		}
		if (!permissions->rlimit.stack.empty()) {
			builder.CreateCall(module->getFunction("nsjail_set_rlimit_stack"), {conf, generateNonConstantString(permissions->rlimit.stack, builder)});
		}

		// Cgroup
		if (permissions->cgroup_mem_max > 0) {
			builder.CreateCall(module->getFunction("nsjail_set_cgroup_mem_max"), {conf, ConstantInt::get(Int64, permissions->cgroup_mem_max)});
		}
		if (permissions->cgroup_pids_max > 0) {
			builder.CreateCall(module->getFunction("nsjail_set_cgroup_pids_max"), {conf, ConstantInt::get(Int64, permissions->cgroup_pids_max)});
		}
		if (permissions->cgroup_cpu_ms_per_sec > 0) {
			builder.CreateCall(module->getFunction("nsjail_set_cgroup_cpu_ms_per_sec"), {conf, ConstantInt::get(Int64, permissions->cgroup_cpu_ms_per_sec)});
		}
	}

	llvm::Value *CommunicationPair::generateNonConstantString(const std::string &data, IRBuilder<> &builder) {
		auto init = ConstantDataArray::getString(module->getContext(), data, true);
		auto var = new GlobalVariable(*module, init->getType(), false, GlobalVariable::PrivateLinkage, init, ".str");
		// Constant* zero = ConstantInt::get(Type::getInt32Ty(module->getContext()), 0);
		// return ConstantExpr::getInBoundsGetElementPtr(var->getType(), var, ArrayRef<Value *>{zero, zero});
		return builder.CreateBitCast(var, Type::getInt8PtrTy(module->getContext()));
	}

	void CommunicationPair::loadUserInstrumentation(const string &name) {
		auto lib = applicationPath.parent_path().append("instrumentations").append("libinstrument-"+name+".so");
		BinaryIpcModule m(lib.string(), false, nullptr, nullptr);
		m.loadModuleFile();
		for (const auto &s: m.getExports()) {
			if (StringRef(s).startswith("instrument_"))
				userInstrumentedFunctions.insert(s);
		}
		newArguments.push_back("-L"+applicationPath.parent_path().append("instrumentations").string());
		newArguments.push_back("-rpath="+applicationPath.parent_path().append("instrumentations").string());
		newArguments.push_back("-linstrument-"+name);
		llvm::errs() << "LOADED " << m.getExports().size() << " EXPORTS\n"; //TODO
	}

	void CommunicationPair::initModule() {
		initModule(LlvmIpcModule::context);
	}



	void IpcCallHandlerFunction::initialize(llvm::Function *function, CommunicationPair &cp) {
		this->function = function;
		this->switchInst = nullptr;
		this->switchEpilog = nullptr;
		this->sendingStruct = nullptr;
		assert(this->function);

		// find switch / case
		for (auto &ins : function->getEntryBlock()) {
			if (isa<SwitchInst>(ins)) {
				this->switchInst = &cast<SwitchInst>(ins);
				break;
			}
		}
		assert(this->switchInst);

		// find epilogue / remove all cases.
		// Convention: last case has to be a "case: break;"
		while (switchInst->case_begin() != switchInst->case_end()) {
			this->switchEpilog = switchInst->case_begin()->getCaseSuccessor();
			switchInst->removeCase(switchInst->case_begin());
		}
		assert(this->switchEpilog);

		// Find a reference to the sending struct
		// Convention: the sending struct is used in the switch/case epilogue
		for (auto &ins : *switchEpilog) {
			if (isa<GetElementPtrInst>(ins)) {
				this->sendingStruct = cast<GetElementPtrInst>(ins).getOperand(0);
				break;
			}
		}
		assert(this->sendingStruct);

		auto diType = cp.diBuilder->createSubroutineType(cp.diBuilder->getOrCreateTypeArray(vector<Metadata *>()));
		this->debug = cp.diBuilder->createFunction(cp.diUnit, function->getName(), StringRef(), cp.diUnit, 0, diType, true, true, 0);
		function->setSubprogram(this->debug);
	}
}

