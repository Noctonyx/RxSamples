# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("RxSamples")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(RXSAMPLES_ASSET_PATH "${PROJECT_SOURCE_DIR}/Assets")

option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)

add_subdirectory("extern/glfw")

option(JSON_BuildTests "Build the unit tests when BUILD_TESTING is enabled." OFF)
add_subdirectory("extern/json")
add_subdirectory("extern/spdlog")

set(imgui_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui)
add_subdirectory("extern/bitsery")

set(tinygltf_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/tinygltf)

add_subdirectory(extern/RmlUi)

option(LUA_BUILD_COMPILER OFF)
add_subdirectory(extern/lua)
add_subdirectory(extern/sol2)
add_subdirectory(extern/freetype)
add_subdirectory(extern/dxmath)

# Include sub-projects.
add_subdirectory("RxUtil")
add_subdirectory("RxCore")
add_subdirectory("RxEngine")
add_subdirectory("RxAssets")
add_subdirectory("RxECS")

add_subdirectory(Triangle)
