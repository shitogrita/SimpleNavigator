add_library(interface STATIC
        console_interface.cpp
        console_interface.h
)

target_include_directories(interface
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)