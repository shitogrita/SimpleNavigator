add_library(core STATIC
    s21_graph.cpp
    s21_graph.h
)

target_include_directories(core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)