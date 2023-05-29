cmake_minimun_required (VERSION 3.0)
project(ticket_system)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(code BPT.hpp main.cpp)