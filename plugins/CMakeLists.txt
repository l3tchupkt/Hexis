cmake_minimum_required(VERSION 3.25)

add_library(example_plugin MODULE example_plugin/example.c)
target_link_libraries(example_plugin PRIVATE hexis)
