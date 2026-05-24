cmake_minimum_required(VERSION 3.13)

project(codecrafters-shell)

enable_testing()

file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.h)

set(CMAKE_C_STANDARD 23) # Enable the C23 standard
set(CMAKE_BUILD_TYPE Debug) # Compile with debug symbols

add_executable(shell ${SOURCE_FILES})

target_link_libraries(shell PRIVATE readline)

add_executable(test_quoting tests/test_quoting.c)
add_test(NAME test_quoting COMMAND test_quoting)
