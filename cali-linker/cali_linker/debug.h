#ifndef LLVMREWRITER_DEBUG_H
#define LLVMREWRITER_DEBUG_H

//#define DEBUGGING

#ifdef DEBUGGING
#define InternalFunctionLinkage GlobalValue::LinkageTypes::ExternalLinkage

#else

#define InternalFunctionLinkage GlobalValue::LinkageTypes::PrivateLinkage

#endif

#define SILENT

#ifdef SILENT
#define dbg_llvm_outs if (0) llvm::outs()
#define dbg_cout if (0) std::cout
#define dbg_cerr if (0) std::cerr
#else
#define dbg_llvm_outs llvm::outs()
#define dbg_cout std::cout
#define dbg_cerr std::cerr
#endif

#endif //LLVMREWRITER_DEBUG_H
