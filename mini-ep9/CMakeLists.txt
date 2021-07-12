cmake_minimum_required(VERSION 3.5)

project(threadpool LANGUAGES C)
set(CMAKE_C_STANDARD 11)
add_executable(threadpool threadpool_fila.c)
target_link_libraries(threadpool pthread)
