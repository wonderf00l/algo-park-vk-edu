SET(GCC_EXTRA "-Wextra")
SET(GCC_ALL "-Wall")
SET(GCC_ERROR "-Werror")
SET(GCC_STANDART "-std=c++17")
SET(GCC_SANITIZE "-fsanitize=address")


SET(CMAKE_CXX_FLAGS "${GCC_EXTRA} ${GCC_ALL} ${GCC_ERROR} ${GCC_STANDART} ${GCC_SANITIZE}")

cmake_minimum_required(VERSION 3.0)
project(ex4 VERSION 0.1.0)


add_subdirectory(comp)
add_subdirectory(heap)



add_executable(ex4 src/main.cpp) #ex4 - target
target_link_libraries(ex4 comps)
target_link_libraries(ex4 heap)