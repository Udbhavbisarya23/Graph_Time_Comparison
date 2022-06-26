add_executable(scc scc.c)
add_executable(parallel_scc parallel_scc.c)

target_link_libraries(scc
    directed_graph)

target_link_libraries(parallel_scc
    directed_graph
    OpenMP::OpenMP_C)