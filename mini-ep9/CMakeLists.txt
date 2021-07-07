cmake_minimum_required(VERSION 3.5)

project(threadpool LANGUAGES C)
add_executable(threadpool threadpool.c)
target_link_libraries(threadpool pthread)
