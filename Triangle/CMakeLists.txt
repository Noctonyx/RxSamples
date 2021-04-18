# CMakeList.txt : CMake project for RX, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

add_executable (Triangle  
    src/Main.cpp 
)

if (MSVC)
    target_compile_options(Triangle PRIVATE /W4 /WX)
endif ()

target_include_directories(Triangle PRIVATE "src")

target_link_libraries(Triangle PRIVATE glfw)
target_link_libraries(Triangle PRIVATE RxEngine)
target_link_libraries(Triangle PRIVATE RxAssets)

#target_link_libraries(Triangle PUBLIC nlohmann_json nlohmann_json::nlohmann_json)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/config.json.in"
        "${CMAKE_CURRENT_BINARY_DIR}/config.json"
)
#add_subdirectory(test)