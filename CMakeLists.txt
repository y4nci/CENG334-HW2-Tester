cmake_minimum_required(VERSION 3.25)
project(tester_hw2)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(tester_hw2
        src/tester-hw2.cpp src/tools/tester_output.cpp src/tools/tester_output.h src/tools/matrix.cpp src/tools/matrix.h src/lib/arguments.cpp src/lib/arguments.h src/tools/TesterModule.cpp src/tools/TesterModule.h src/lib/Fork.cpp src/lib/Fork.h)
