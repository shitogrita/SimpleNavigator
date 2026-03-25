add_library(graph STATIC
    s21_graph.cpp
    s21_graph.h
)

target_include_directories(graph
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)