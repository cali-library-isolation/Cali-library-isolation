cmake_minimum_required(VERSION 3.5)
project(Cali)

add_subdirectory(cali-linker)
add_subdirectory(libipc)
add_subdirectory(libnsjail)
add_subdirectory(instrumentations)
add_dependencies(cali ipc)
add_dependencies(cali ipc-mprotect)
add_dependencies(cali ipc-sequential)
add_dependencies(cali nsjail)


# copy precompiled libraries
add_custom_command(OUTPUT libstdc++.bc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/precompiled-libraries/libstdc++.bc
        ${CMAKE_CURRENT_BINARY_DIR}/libstdc++.bc)
add_custom_target(precompiled-libraries ALL DEPENDS libstdc++.bc)