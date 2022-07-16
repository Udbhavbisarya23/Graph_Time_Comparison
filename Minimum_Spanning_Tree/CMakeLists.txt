add_executable(kruskal kruskal.c)
add_executable(parallel_kruskal parallel_kruskal.c)
add_executable(helper helper.c)

target_link_libraries(kruskal
    format_file
    undirected_graph
    map
    disjoint_sets)

target_link_libraries(parallel_kruskal
    format_file
    undirected_graph
    map
    disjoint_sets
    OpenMP::OpenMP_C)

target_link_libraries(helper
    generate_graph)