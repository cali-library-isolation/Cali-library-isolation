cmake_minimum_required(VERSION 3.7)
project(llvmrewriter)

set(CMAKE_CXX_STANDARD 14)

#add_definitions(-fno-rtti)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

# import LLVM
#include(../../llvm-4.0/install/lib/cmake/llvm/LLVMConfig.cmake REQUIRED)
find_package(LLVM 7 REQUIRED)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
message("LLVM version: ${LLVM_VERSION}")

# prepare clang compiler for runtime libraries
find_program(LLVM_BC_CXX_COMPILER "clang-7")
if (NOT LLVM_BC_CXX_COMPILER)
	message(SEND_ERROR "Some of following tools have not been found:")
	message(SEND_ERROR "LLVM_BC_CXX_COMPILER")
endif ()
set(LLVM_BC_C_FLAGS -c -O2 -flto -fno-stack-protector)
set(LLVM_BC_CXX_FLAGS -std=c++11 -c -O3 -flto -fno-stack-protector)


# import boost
find_package(Boost COMPONENTS filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
find_package(LibArchive REQUIRED)
include_directories(LibArchive_INCLUDE_DIRS)


# configure clang's sanitizers (build with -DCMAKE_BUILD_TYPE=addrsanitizer / memsanitizer)
SET(CMAKE_C_FLAGS_ADDRSANITIZER "-fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fno-optimize-sibling-calls -O1 -g")
SET(CMAKE_CXX_FLAGS_ADDRSANITIZER "-fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fno-optimize-sibling-calls -O1 -g")
SET(CMAKE_C_FLAGS_MEMSANITIZER "-fsanitize=memory -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize-memory-track-origins -O1 -g")
SET(CMAKE_CXX_FLAGS_MEMSANITIZER "-fsanitize=memory -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize-memory-track-origins -O1 -g")
IF ("${CMAKE_BUILD_TYPE}" STREQUAL "addrsanitizer" OR "${CMAKE_BUILD_TYPE}" STREQUAL "memsanitizer")
	message("--- SANITIZER BUILD CONFIGURED ---")
	set(CMAKE_C_COMPILER clang-9)
	set(CMAKE_CXX_COMPILER clang++-9)
ENDIF ()


set(SOURCE_FILES
		cali_linker/archive-wrapper.cpp
		cali_linker/archive-wrapper.h
		cali_linker/configfile.cpp
		cali_linker/configfile.h
		cali_linker/configfile_generator.cpp
		cali_linker/configfile_generator.h
		cali_linker/debug.h
		cali_linker/ipc_rewriter.cpp
		cali_linker/ipc_rewriter.h
		cali_linker/timing-utils.h
		cali_linker/timing_utils.cpp
		graph/abstract_map.h
		graph/algorithms.h
		graph/basegraph.cpp
		graph/basegraph.h
		graph/color_map.h
		graph/dotwriter.h
		graph/scc_algorithms.h
		graph/unique_vector.h
		modules/binary_module.cpp
		modules/communication_pair.cpp
		modules/communication_pair.h
		modules/ipc_module.cpp
		modules/ipc_module.h
		modules/llvm_module.cpp
		modules/llvm_module.h
		passes/DataDrivenSCCPass.cpp
		passes/DataDrivenSCCPass.h
		passes/FunctionAliasResolverPass.cpp
		passes/FunctionAliasResolverPass.h
		passes/PDG.cpp
		passes/PDG.h
		passes/PDGCreationPass.cpp
		passes/PDGCreationPass.h
		passes/PDGInstrumentPass.cpp
		passes/PDGInstrumentPass.h
		passes/PDGReachabilityPass.cpp
		passes/PDGReachabilityPass.h
		passes/PDGSharedMemoryPass.cpp
		passes/PDGSharedMemoryPass.h
		passes/PDGSharedReachabilityPass.cpp
		passes/PDGSharedReachabilityPass.h
		passes/PDGSpecializationPass.cpp
		passes/PDGSpecializationPass.h
		passes/PDGUtilities.cpp
		passes/PDGUtilities.h
		passes/PDGWriterPass.cpp
		passes/PDGWriterPass.h
		passes/SimpleSharedAnalysisPass.cpp
		passes/SimpleSharedAnalysisPass.h
		stdlib-is-shit.h
		cali_linker/randomness.cpp
		cali_linker/randomness.h
		)
set(SOURCE_FILES_CALI
		cali_linker/linker_replacement.cpp
		cali_linker/linker_replacement.h
		)


add_executable(cali ${SOURCE_FILES} ${SOURCE_FILES_CALI})
#target_link_libraries(cali LLVMCore LLVMIRReader LLVMBitWriter)
target_link_libraries(cali LLVM)
target_link_libraries(cali ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${LibArchive_LIBRARIES})


# find libraries
set(libipc_DIR ../libipc)
set(libnsjail_DIR ../libnsjail)
find_package(libipc REQUIRED)
include_directories(${LIBIPC_INCLUDE_DIRS})
find_package(libnsjail REQUIRED)
include_directories(${LIBNSJAIL_INCLUDE_DIRS})

# build prototype modules
message("BC Compiler command: ${LLVM_BC_CXX_COMPILER} -I${LIBIPC_INCLUDE_DIRS} -I${LIBNSJAIL_INCLUDE_DIRS} ${LLVM_BC_CXX_FLAGS} -o ipc-communication-proto.bc ${CMAKE_CURRENT_SOURCE_DIR}/snippets/ipc-communication-proto.cpp")
add_custom_command(OUTPUT ipc-communication-proto.bc
		COMMAND "${LLVM_BC_CXX_COMPILER}" "-I${LIBIPC_INCLUDE_DIRS}" "-I${LIBNSJAIL_INCLUDE_DIRS}" ${LLVM_BC_CXX_FLAGS} -o ipc-communication-proto.bc ${CMAKE_CURRENT_SOURCE_DIR}/snippets/ipc-communication-proto.cpp
		MAIN_DEPENDENCY snippets/ipc-communication-proto.cpp)
add_custom_command(OUTPUT libc-stubs.bc
		COMMAND "${LLVM_BC_CXX_COMPILER}" ${LLVM_BC_C_FLAGS} -o libc-stubs.bc ${CMAKE_CURRENT_SOURCE_DIR}/snippets/libc-stubs.c
		MAIN_DEPENDENCY snippets/libc-stubs.c)
add_custom_target(snippets ALL DEPENDS ipc-communication-proto.bc libc-stubs.bc)

# add_dependencies(ipc-modularizer snippets)
add_dependencies(cali snippets)

