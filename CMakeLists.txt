cmake_minimum_required (VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project(n-times-two-squares)
add_executable(n-times-two-squares main.cpp)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
target_link_libraries(n-times-two-squares ${SDL2_LIBRARIES})