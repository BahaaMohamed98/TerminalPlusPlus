cmake_minimum_required(VERSION 3.28)
project(TerminalPlusPlus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

include_directories(src)

add_executable(example src/examples/example.cpp)
add_executable(CountDown src/examples/countDown.cpp)
add_executable(test src/test.cpp)