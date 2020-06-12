cmake_minimum_required (VERSION 3.14)

project (recipe VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")
include(utils)

add_subdirectory(recipe)
add_subdirectory(io)
add_subdirectory(documentation)


enable_testing()
find_package(GTest REQUIRED)
add_subdirectory(test)
