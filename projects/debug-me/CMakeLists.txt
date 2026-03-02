cmake_minimum_required(VERSION 3.10)
project(debug-me C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
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
pkg_check_modules(CRITERION REQUIRED IMPORTED_TARGET criterion)
message(STATUS "Criterion include dirs: ${CRITERION_INCLUDE_DIRS}")
message(STATUS "Criterion cflags: ${CRITERION_CFLAGS}")

# Find all C files in the directory
file(GLOB SOURCES "*.c")

# Separate out main.c and test.c
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.c")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test.c")

# Main Executable (we don't have a library yet since there are no other files)
add_executable(debug-me main.c)

# Test Executable
add_executable(test test.c)
target_link_libraries(test ${CRITERION_LIBRARIES})
target_compile_options(test PRIVATE ${CRITERION_CFLAGS})

install(TARGETS debug-me DESTINATION bin)
install(TARGETS test DESTINATION bin)
