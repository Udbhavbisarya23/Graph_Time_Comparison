add_library(map map.c map.h)

add_library(disjoint_sets disjoint_sets.c disjoint_sets.h)

add_library(queue queue.c queue.h)
target_link_libraries(queue
    directed_graph)