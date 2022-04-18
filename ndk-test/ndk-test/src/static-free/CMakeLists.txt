cmake_minimum_required(VERSION 3.16)

project(FREE_STATIC)

add_executable(free free-static.cpp)
target_link_options(free PUBLIC "-static")
