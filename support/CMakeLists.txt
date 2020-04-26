cmake_minimum_required(VERSION 3.12)

set(SUPPORT_LIB_SOURCES error.cpp)

add_library(support STATIC ${SUPPORT_LIB_SOURCES})
target_compile_options(support PUBLIC -Wall -Werror -Wextra -O3)