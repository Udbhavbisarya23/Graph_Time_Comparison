add_library(undirected_graph undirected_graph.c undirected_graph.h)
add_library(directed_graph directed_graph.c directed_graph.h)
add_library(bipartite_graph bipartite_graph.c bipartite_graph.h)

target_link_libraries(bipartite_graph
    map
    OpenMP::OpenMP_C
    Threads::Threads)