cmake_minimum_required(VERSION 3.5)
project(Cali)

add_subdirectory(cali-linker)
add_subdirectory(libipc)
add_subdirectory(libnsjail)


# copy precompiled libraries
add_custom_command(OUTPUT libstdc++.bc POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/precompiled-libraries/libstdc++.bc
        ${CMAKE_CURRENT_BINARY_DIR}/libstdc++.bc)
add_custom_target(precompiled-libraries ALL DEPENDS libstdc++.bc)