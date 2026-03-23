cmake_minimum_required(VERSION 3.10)
project(w4tute C)

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

# Main Executable
add_executable(w4tute main.c)

# Test Executable
add_executable(test test.c)
target_link_libraries(test ${CRITERION_LIBRARIES})
target_include_directories(test PUBLIC ${CRITERION_INCLUDE_DIRS})
target_compile_options(test PUBLIC ${CRITERION_CFLAGS_OTHER})

install(TARGETS w4tute DESTINATION bin)
install(TARGETS test DESTINATION bin)
