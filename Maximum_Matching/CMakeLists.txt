add_executable(maxMatching maxMatching.c)
add_executable(biPreprocess bipartitePreprocessing.c)

target_link_libraries(maxMatching
    format_file
    bipartite_graph
    )

target_link_libraries(biPreprocess
    generate_graph)
