add_library(algorithms STATIC
        s21_graph_algorithms.cpp
        s21_graph_algorithms.h
)

target_include_directories(algorithms
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)