cmake_minimum_required(VERSION 3.10.0)
project(vcs)

file(GLOB_RECURSE VCS_LIB_SOURCES "src/lib/*.cpp" "src/lib/*.c")
file(GLOB_RECURSE VCS_DAEMON_SOURCES "src/daemon/*.cpp" "src/daemon/*.c")

include_directories(src/lib)
include_directories(include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 11)

add_library(vcslib STATIC ${VCS_LIB_SOURCES})

add_executable(vcsdaemon ${VCS_DAEMON_SOURCES})
target_link_libraries(vcsdaemon vcslib)

