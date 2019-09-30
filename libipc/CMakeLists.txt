cmake_minimum_required(VERSION 3.7)
project(libipc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

# dummy library to resolve symbols from the application
add_library(link_resolve_dummy SHARED "linker-resolve-dummy.c")


# Locate dependencies (libnsjail)
set(libnsjail_DIR ../libnsjail)
find_package(libnsjail REQUIRED)
include_directories(${LIBNSJAIL_INCLUDE_DIRS})


# Prepare compiler options for our malloc() fork
set(LIBC_SOURCE_DIR "${CMAKE_SOURCE_DIR}/glibc-2.23")
set(LIBC_SOURCE_DIR_DEBIAN "${LIBC_SOURCE_DIR}/debian")
set(LIBC_SOURCE_DIR_BUILD "${LIBC_SOURCE_DIR}/build-tree")

set(MALLOC_C_FLAGS "${CMAKE_C_FLAGS} \
	-fgnu89-inline \
	-fno-stack-protector \
	-O3 \
	-Wall \
	-Werror \
	-Wundef \
	-Wwrite-strings \
	-fmerge-all-constants \
	-frounding-math \
	-g \
	-pipe \
	-Wstrict-prototypes \
	-Wold-style-definition \
	-DMORECORE_CLEARS=2 \
	-ftls-model=initial-exec \
	-U_FORTIFY_SOURCE \
	-I${CMAKE_CURRENT_SOURCE_DIR}/include \
	-I${CMAKE_CURRENT_SOURCE_DIR} \
	-isystem ${LIBC_SOURCE_DIR_DEBIAN}/include \
	-I${LIBC_SOURCE_DIR}/include \
	-I${LIBC_SOURCE_DIR_BUILD}/amd64-libc/malloc \
	-I${LIBC_SOURCE_DIR_BUILD}/amd64-libc \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/x86_64/64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/x86_64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/x86 \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/wordsize-64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/nptl \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/include \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux \
	-I${LIBC_SOURCE_DIR}/sysdeps/nptl \
	-I${LIBC_SOURCE_DIR}/sysdeps/pthread \
	-I${LIBC_SOURCE_DIR}/sysdeps/gnu \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/inet \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/x86_64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix \
	-I${LIBC_SOURCE_DIR}/sysdeps/posix \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/fpu/multiarch \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/fpu \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86/fpu/include \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86/fpu \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/multiarch \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754/ldbl-96 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754/dbl-64/wordsize-64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754/dbl-64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754/flt-32 \
	-I${LIBC_SOURCE_DIR}/sysdeps/wordsize-64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754 \
	-I${LIBC_SOURCE_DIR}/sysdeps/generic \
	-I${LIBC_SOURCE_DIR} \
	-I${LIBC_SOURCE_DIR}/libio \
	-I${LIBC_SOURCE_DIR}/malloc \
	-nostdinc \
	-isystem /usr/lib/gcc/x86_64-linux-gnu/5/include \
	-isystem /usr/lib/gcc/x86_64-linux-gnu/5/include-fixed \
	-isystem ${LIBC_SOURCE_DIR_DEBIAN}/include \
	-D_LIBC_REENTRANT \
	-include ${LIBC_SOURCE_DIR_BUILD}/amd64-libc/libc-modules.h \
	-DMODULE_NAME=shmmalloc \
	-DMODULE_shmmalloc=12345 \
	-include ${LIBC_SOURCE_DIR}/include/libc-symbols.h \
")

set(MALLOC_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
	-fno-stack-protector \
	-O3 \
	-Wall \
	-Werror \
	-Wundef \
	-Wwrite-strings \
	-fmerge-all-constants \
	-frounding-math \
	-g \
	-pipe \
	-DMORECORE_CLEARS=2 \
	-ftls-model=initial-exec \
	-U_FORTIFY_SOURCE \
	-I${CMAKE_CURRENT_SOURCE_DIR}/include \
	-I${CMAKE_CURRENT_SOURCE_DIR} \
	-isystem ${LIBC_SOURCE_DIR_DEBIAN}/include \
	-I${LIBC_SOURCE_DIR}/include \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/x86_64/64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/x86_64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/x86 \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/wordsize-64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/nptl \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux/include \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv/linux \
	-I${LIBC_SOURCE_DIR}/sysdeps/nptl \
	-I${LIBC_SOURCE_DIR}/sysdeps/pthread \
	-I${LIBC_SOURCE_DIR}/sysdeps/gnu \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/inet \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/sysv \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix/x86_64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/unix \
	-I${LIBC_SOURCE_DIR}/sysdeps/posix \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/fpu/multiarch \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/fpu \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86/fpu/include \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86/fpu \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64/multiarch \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86_64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/x86 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754/ldbl-96 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754/dbl-64/wordsize-64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754/dbl-64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754/flt-32 \
	-I${LIBC_SOURCE_DIR}/sysdeps/wordsize-64 \
	-I${LIBC_SOURCE_DIR}/sysdeps/ieee754 \
	-I${LIBC_SOURCE_DIR}/sysdeps/generic \
	-I${LIBC_SOURCE_DIR} \
	-I${LIBC_SOURCE_DIR}/libio \
	-I${LIBC_SOURCE_DIR}/malloc \
	-nostdinc \
	-isystem /usr/lib/gcc/x86_64-linux-gnu/5/include \
	-isystem /usr/lib/gcc/x86_64-linux-gnu/5/include-fixed \
	-isystem ${LIBC_SOURCE_DIR_DEBIAN}/include \
	-D_LIBC_REENTRANT \
	-DMODULE_NAME=shmmalloc \
	-DMODULE_shmmalloc=12345 \
	-include ${LIBC_SOURCE_DIR}/include/libc-symbols.h \
	-include ${LIBC_SOURCE_DIR}/include/libc-symbols.h \
")

# ipc sources - static_resources last
set(SOURCE_FILES
		debug.h
		file_descriptor_manager.cpp
		hook.hpp
		include/libipc/annotations.h
		ipc_communication.cpp
		ipc_communication.h
		ipc_constants.cpp
		mappings.cpp
		mappings.h
		shared_lock.h
		shared_memory_manager.cpp
		shared_memory_manger.h
		)
# malloc sources
set(SOURCE_FILES ${SOURCE_FILES}
		byte_bitset.h
		libc_fatal.c
		malloc/arena.c.h
		malloc/hooks.c.h
		malloc/linkrepair_c.c
		malloc/malloc.c
		malloc/malloc.h
		malloc/renamed_functions.h
		other_inclues/malloc.h
		shm_malloc.h
		)
set(SOURCE_FILES ${SOURCE_FILES}
		malloctest/malloc_tester.cpp
		malloctest/malloc_tester.h
		malloctest/order_statistic_tree.h
		)
set(SOURCE_FILES_INSTRUMENT
		instrument/instrumentation.cpp
		instrument/IntervalTree.h
		)
add_library(ipc STATIC ${SOURCE_FILES})
add_library(ipc-instrument STATIC ${SOURCE_FILES} ${SOURCE_FILES_INSTRUMENT})

# special properties for libc-forked files
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/malloc/malloc.c PROPERTIES COMPILE_FLAGS "${MALLOC_C_FLAGS}")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/libc_fatal.c PROPERTIES COMPILE_FLAGS "${MALLOC_C_FLAGS}")
#set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/malloc/linkrepair_cxx.cpp PROPERTIES COMPILE_FLAGS "${MALLOC_CXX_FLAGS}")
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/malloc/linkrepair_c.c PROPERTIES COMPILE_FLAGS "${MALLOC_C_FLAGS}")

target_link_libraries(ipc rt dl pthread link_resolve_dummy)

