add_library(format_file format_file.c format_file.h)
add_library(generate_graph generate_graph.c generate_graph.h)

target_link_libraries(format_file
    map)