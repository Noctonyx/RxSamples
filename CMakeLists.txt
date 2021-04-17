# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("RxSamples")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(RXSAMPLES_ASSET_PATH "${PROJECT_SOURCE_DIR}/Assets")

add_subdirectory("RxEngine")
add_subdirectory(Triangle)
