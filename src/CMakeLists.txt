cmake_minimum_required(VERSION 3.16)

project(SimpleNavigator)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(GTest REQUIRED)

add_subdirectory(graph)
add_subdirectory(containers)
add_subdirectory(algorithms)
add_subdirectory(interface)

add_executable(Main main.cpp)

target_include_directories(Main PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/graph
        ${CMAKE_CURRENT_SOURCE_DIR}/containers
        ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        ${CMAKE_CURRENT_SOURCE_DIR}/interface
)

target_link_libraries(Main PRIVATE
        graph
        containers
        algorithms
        interface
)

add_executable(run_test
        run_tests.cpp
)

target_include_directories(run_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/graph
        ${CMAKE_CURRENT_SOURCE_DIR}/containers
        ${CMAKE_CURRENT_SOURCE_DIR}/algorithms
        ${CMAKE_CURRENT_SOURCE_DIR}/test
)

target_link_libraries(run_test PRIVATE
        graph
        containers
        algorithms
        GTest::gtest
        GTest::gtest_main
)