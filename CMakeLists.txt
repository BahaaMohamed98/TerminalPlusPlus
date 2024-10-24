cmake_minimum_required(VERSION 3.28)
project(TerminalPlusPlus)

set(CMAKE_CXX_STANDARD 17)

add_executable(TerminalPlusPlus src/example.cpp
        src/Terminal++.hpp)

include_directories(src)
