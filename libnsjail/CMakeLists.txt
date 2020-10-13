cmake_minimum_required(VERSION 3.5)
project(libnsjail)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_definitions(-D_GNU_SOURCE)
# from original nsjail makefile
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-all -O2")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}")
find_package(NL REQUIRED)
include_directories(${NL_INCLUDE_DIRS})


SET(Project_Include_Dir "${PROJECT_SOURCE_DIR}/include")
SET(Project_Source_Dir "${PROJECT_SOURCE_DIR}/src")

include_directories(
		"${PROJECT_SOURCE_DIR}/include"
		"${PROJECT_SOURCE_DIR}/include/nsjail"
		"${PROJECT_SOURCE_DIR}/src"
		"${PROJECT_SOURCE_DIR}/src/nsjail/kafel/include"
)

set(SOURCE_FILES include/nsjail/fork.h include/nsjail/common.h src/libnsjail/cmdline_utils.h)
set(SOURCE_FILES ${SOURCE_FILES} src/libnsjail/fork.cc src/libnsjail/api.cc)
set(SOURCE_FILES ${SOURCE_FILES}
		src/nsjail/caps.cc
		src/nsjail/caps.h
		src/nsjail/cgroup.cc
		src/nsjail/cgroup.h
		src/nsjail/cgroup2.cc
		src/nsjail/cgroup2.h
		src/nsjail/cmdline.cc
		src/nsjail/cmdline.h
		src/nsjail/config.cc
		src/nsjail/config.h
		#src/nsjail/config.pb.cc
		#src/nsjail/config.pb.h
		src/nsjail/contain.cc
		src/nsjail/contain.h
		src/nsjail/cpu.cc
		src/nsjail/cpu.h
		src/nsjail/kafel/include/kafel.h
		src/nsjail/logs.cc
		src/nsjail/logs.h
		src/nsjail/macros.h
		src/nsjail/mnt.cc
		src/nsjail/mnt.h
		src/nsjail/net.cc
		src/nsjail/net.h
		src/nsjail/nsjail.h
		src/nsjail/pid.cc
		src/nsjail/pid.h
		src/nsjail/sandbox.cc
		src/nsjail/sandbox.h
		src/nsjail/subproc.cc
		src/nsjail/subproc.h
		src/nsjail/user.cc
		src/nsjail/user.h
		src/nsjail/util.cc
		src/nsjail/util.h
		src/nsjail/uts.cc
		src/nsjail/uts.h
		)
add_library(nsjail ${SOURCE_FILES})


# Kafel dependency
add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/nsjail/kafel/libkafel.a
		COMMAND make
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/nsjail/kafel
		)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libkafel.a DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/nsjail/kafel/libkafel.a COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/src/nsjail/kafel/libkafel.a
		${CMAKE_CURRENT_BINARY_DIR}/libkafel.a
		)
add_custom_target(libkafel DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libkafel.a)
add_dependencies(nsjail libkafel)

