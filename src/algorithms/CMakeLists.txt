add_library(algorithms STATIC
        s21_graph_algorithms.cpp
        s21_graph_algorithms.h
        s21_tsp_compare.cpp
        s21_tsp_compare.h
)

target_include_directories(algorithms
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)