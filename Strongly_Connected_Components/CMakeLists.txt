add_executable(scc scc.c)
add_executable(parallel_scc parallel_scc.c)

target_link_libraries(scc
    directed_graph
    queue)

target_link_libraries(parallel_scc
    directed_graph
    queue
    OpenMP::OpenMP_C)