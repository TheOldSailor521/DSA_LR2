cmake_minimum_required(VERSION 4.2)
project(DSA_LR2)

set(CMAKE_CXX_STANDARD 20)

add_executable(DSA_LR2 main.cpp hashtable.cpp order.cpp)
configure_file(input.txt input.txt COPYONLY)
