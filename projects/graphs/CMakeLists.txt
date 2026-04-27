cmake_minimum_required(VERSION 3.10)
project(graphs C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Custom flags for build types
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} -O1 -U_FORTIFY_SOURCE -g3 -Wall -Wextra -glldb -fsanitize=address,undefined -Werror -Wno-newline-eof -Wno-unused-parameter -pedantic -std=c17"
    )
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(CRITERION REQUIRED criterion)

# Find all C files in the directory
file(GLOB SOURCES "*.c")

# Separate out main.c and test.c
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.c")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test.c")

# Create a library from the common sources so they can be shared
add_library(graphs_lib STATIC ${SOURCES})

# Main Executable
add_executable(graphs main.c)
target_link_libraries(graphs graphs_lib)

# Test Executable
add_executable(test test.c)
target_link_libraries(test graphs_lib ${CRITERION_LIBRARIES})
target_include_directories(test PUBLIC ${CRITERION_INCLUDE_DIRS})
target_compile_options(test PUBLIC ${CRITERION_CFLAGS_OTHER})

install(TARGETS graphs DESTINATION bin)
install(TARGETS test DESTINATION bin)
