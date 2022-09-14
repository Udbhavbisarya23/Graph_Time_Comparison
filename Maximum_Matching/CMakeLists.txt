add_executable(maxMatching maxMatching.c)
add_executable(parallelMaxMatching parallelMaxMatching.c)
add_executable(biPreprocess bipartitePreprocessing.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(maxMatching PRIVATE
    format_file
    bipartite_graph
    OpenMP::OpenMP_C)

target_link_libraries(parallelMaxMatching PRIVATE
    format_file
    bipartite_graph
    OpenMP::OpenMP_C
    Threads::Threads)

target_link_libraries(biPreprocess PRIVATE
    generate_graph)
