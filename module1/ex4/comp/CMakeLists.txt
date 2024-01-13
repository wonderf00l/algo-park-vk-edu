cmake_minimum_required(VERSION 3.0)


add_library(comps src/comp.cpp) #comps - target
target_include_directories(comps PUBLIC include)
