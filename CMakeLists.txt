cmake_minimum_required(VERSION 3.10)
project(box2d_try CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

find_package(box2d REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB IMGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

add_executable(box2d_try windows.cpp)

target_include_directories(box2d_try PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${GLFW3_INCLUDE_DIRS}
)

target_link_libraries(box2d_try box2d m imgui glfw3 OpenGL)