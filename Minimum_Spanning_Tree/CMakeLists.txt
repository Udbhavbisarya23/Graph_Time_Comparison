add_executable(kruskal kruskal.c)

target_link_libraries(kruskal
    format_file
    undirected_graph
    map)