cmake_minimum_required(VERSION 3.10.0)
project(parallelizing_graphs)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

find_package(OpenMP REQUIRED)
find_package(Threads REQUIRED)
add_subdirectory(Maximum_Matching)
add_subdirectory(Minimum_Spanning_Tree)
add_subdirectory(Strongly_Connected_Components)

add_subdirectory(Utilities)