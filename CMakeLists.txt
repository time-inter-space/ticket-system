cmake_minimum_required (VERSION 3.22)
project(ticket_system)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(src)

add_executable(code src/BPT.hpp src/main.cpp)