#ifndef LLVMREWRITER_COMMUNICATION_PAIR_H
#define LLVMREWRITER_COMMUNICATION_PAIR_H

#include <llvm/IR/Module.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include <unordered_map>
#include <llvm/IR/Value.h>
#include "ipc_module.h"
#include "../cali_linker/configfile.h"

namespace llvm {
	class Function;

	class LLVMContext;

	class ConstantFolder;

	class IRBuilderDefaultInserter;

	class CallInst;

	class DIBuilder;
}

namespace ipcrewriter {

	class IpcCallHandlerFunction {
	public:
		llvm::Function *function;
		llvm::SwitchInst *switchInst;
		llvm::BasicBlock *switchEpilog;
		llvm::Value *sendingStruct;
		llvm::DISubprogram *debug;

		void initialize(llvm::Function *function, CommunicationPair &cp);
	};

	struct WrapperFunctionInfos {
		llvm::Function *original;
		llvm::Function *wrapper;
		unsigned int code;
		bool inProcessIpcCall[2] = {false, false};

		WrapperFunctionInfos() : original(nullptr), wrapper(nullptr), code(0) {};

		WrapperFunctionInfos(llvm::Function *original, llvm::Function *wrapper, unsigned int code) : original(original), wrapper(wrapper), code(code) {};
	};

	class CommunicationPair {
		friend IpcCallHandlerFunction;
	protected:
		bool concurrentLibraryCommunication;
		std::vector<std::string> logEntries;
		static int next_id;
		int id;
		unsigned int next_code;
		IpcModule *module1;
		IpcModule *module2;
		llvm::StructType *basicCommMemType;
		static constexpr int basicCommMemTypeIdxData = 3;
		std::unique_ptr<llvm::Module> module;
		IpcCallHandlerFunction processIpcCall1;
		IpcCallHandlerFunction processIpcCall2;
		llvm::Function *wrapOutgoingCallback;
		llvm::Function *wrapIncomingCallback;
		llvm::Function *wrapOutgoingVaList;
		llvm::Function *wrapPointerFromCallback;
		llvm::Function *unwrapPointerFromCallback;
		llvm::Function *nsjailConfigure;
		llvm::Function *ipcShareFD;
		llvm::Function *ipcLockSending;

		// shm_malloc.h and other libraries
		llvm::Type *i8PtrType;
		llvm::Function *shm_malloc = nullptr;
		llvm::Function *memcpy;
		llvm::Function *free;

		std::unique_ptr<llvm::DIBuilder> diBuilder;
		llvm::DIFile *diUnit;


		std::unordered_map<std::string, WrapperFunctionInfos *> wrappedFunctions;
		std::unordered_map<llvm::Type *, llvm::Function *> callbackTypeHandlers;
		std::unordered_map<llvm::Type *, llvm::Function *> callbackTypeReplacements;

		std::set<std::string> userInstrumentedFunctions;

		const std::map<std::string, std::vector<int>> *fileDescriptorPolicy = nullptr;

		void initModule(llvm::LLVMContext &context);

		void createInternalReplacements();

		template <class InstTy>
		WrapperFunctionInfos *createReplacementFunction(std::string &name, llvm::Function *originalFunction, InstTy *callInst);

		/**
		 *
		 * @param wfi
		 * @param hf reference to processIpcCall function information
		 * @param processIpcCallId 0 or 1 (for function processIpcCall1 or 2)
		 */
		void createReplacementHandler(WrapperFunctionInfos &wfi, IpcCallHandlerFunction &hf, int processIpcCallId);

		llvm::Value *wrapOutgoingValue(llvm::Value *value, llvm::IRBuilder<> &builder);

		void freeWrapOutgoingValueResult(llvm::Value *argument, llvm::Value *wrapped, llvm::IRBuilder<> &builder);

		llvm::Value *wrapIncomingValue(llvm::Value *value, llvm::IRBuilder<> &builder);

		llvm::Function *getCallbackTypeHandler(llvm::Type *type);

		llvm::Function *getCallbackTypeReplacement(llvm::Type *type);

		llvm::Value *generateNonConstantString(const std::string &data, llvm::IRBuilder<> &builder);

	public:
		CommunicationPair(IpcModule *module1, IpcModule *m2, bool concurrentLibraryCommunication);

		~CommunicationPair();

		void initModule();

		int getId() const;

		template <class InstTy>
		llvm::Function *getOrCreateWrapperFunction(llvm::Function *callee, IpcModule *sendingModule, InstTy *callInst);

		void setNsjailConfig(const PermissionConfig *permissions);

		void loadUserInstrumentation(const std::string& name);

		inline void setFileDescriptorPolicy(const std::map<std::string, std::vector<int>> *fileDescriptorPolicy) {
			this->fileDescriptorPolicy = fileDescriptorPolicy;
		}

		void save(const std::string &filename);

		inline const std::vector<std::string> &getLogEntries() const {
			return logEntries;
		}

		std::vector<std::string> newArguments;
	};

}

#endif //LLVMREWRITER_COMMUNICATION_PAIR_H
