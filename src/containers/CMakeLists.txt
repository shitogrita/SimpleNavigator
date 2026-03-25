add_library(containers STATIC
        allocator.cpp
        allocator.h
        queue.h
        stack.h
)

target_include_directories(containers
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)