cmake_minimum_required(VERSION 3.10.0)
project(parallelizing_graphs)

find_package(OpenMP REQUIRED)
add_subdirectory(Minimum_Spanning_Tree)

add_subdirectory(Utilities)