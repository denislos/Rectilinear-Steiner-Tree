cmake_minimum_required(VERSION 3.12)

project(rectilinear-steiner-tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)


find_package(OpenMP)
find_package(Boost)


# RapidXML library
include_directories(rapidxml-1.13)

#Support Lib
add_subdirectory(support)
include_directories(support)

include_directories(src)

set(SOURCES src/program.cpp
            src/hanan_grid.cpp
            src/grid_layers.cpp
            src/layered_grid.cpp
            src/rect_steiner_tree.cpp
            src/minimum_spanning_tree.cpp
            src/rect_steiner_tree_factory.cpp)

add_executable(steiner ${SOURCES})
target_link_libraries(steiner PUBLIC support)
target_link_libraries(steiner PUBLIC OpenMP::OpenMP_CXX)
target_compile_options(steiner PUBLIC -Wall -Werror -Wextra -O3)
