cmake_minimum_required(VERSION 4.2)
project(DSA_LR2)

set(CMAKE_CXX_STANDARD 20)

add_executable(DSA_LR2 main.cpp hashtable.cpp order.cpp)
configure_file(input.txt input.txt COPYONLY)

enable_testing()
add_executable(HashTableTests tests/hashtabletests.cpp hashtable.cpp order.cpp)
target_include_directories(HashTableTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME HashTableTests COMMAND HashTableTests)

add_executable(OrderFileTests tests/orderfiletests.cpp hashtable.cpp order.cpp)
target_include_directories(OrderFileTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME OrderFileTests COMMAND OrderFileTests)
