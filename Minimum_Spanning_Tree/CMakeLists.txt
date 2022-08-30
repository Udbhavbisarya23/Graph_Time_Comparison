add_executable(kruskal kruskal.c)
add_executable(parallel_kruskal parallel_kruskal.c)
add_executable(helper helper.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

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
    OpenMP::OpenMP_C
    Threads::Threads)

target_link_libraries(helper
    generate_graph)