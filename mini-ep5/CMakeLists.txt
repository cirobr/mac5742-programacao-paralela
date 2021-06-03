cmake_minimum_required(VERSION 3.5)

project(mini-ep5 LANGUAGES C)

add_executable(mini-ep5 ep2.c)

TARGET_LINK_LIBRARIES(mini-ep5 pthread)
