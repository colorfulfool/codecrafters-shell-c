cmake_minimum_required(VERSION 3.13)

project(codecrafters-shell)

file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.h)

set(CMAKE_C_STANDARD 23) # Enable the C23 standard
set(CMAKE_BUILD_TYPE Release) # Compile with debug symbols

add_executable(shell ${SOURCE_FILES})

target_link_libraries(shell PRIVATE readline)
