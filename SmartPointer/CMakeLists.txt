cmake_minimum_required(VERSION 3.10)
project(SmartPointer)

set(CMAKE_CXX_STANDARD 11)

add_executable(SmartPointerTest test/main.cpp include/SmartPointer.h src/ReferenceCounter.cpp)
