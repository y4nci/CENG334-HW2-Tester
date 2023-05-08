cmake_minimum_required(VERSION 3.25)
project(tester_hw2)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(tester_hw2
        src/tester.cpp src/tester_output.cpp src/tester_output.h src/tools/matrix.cpp src/tools/matrix.h src/tools/thread.cpp src/tools/thread.h src/lib/arguments.cpp src/lib/arguments.h src/lib/execute.cpp src/lib/execute.h)
