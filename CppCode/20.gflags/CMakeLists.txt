cmake_minimum_required(VERSION 3.10)


project(flags)

find_package(gflags REQUIRED)
target_link_libraries(flags PRIVATE gflags)


add_executable(flags main.cc)

