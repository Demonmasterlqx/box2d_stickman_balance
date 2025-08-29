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

file(GLOB Renderer_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/*.cpp
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${GLFW3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer
)

add_library(imgui STATIC ${IMGUI_SOURCES})
add_library(renderer STATIC ${Renderer_SOURCES})

add_executable(box2d_try demo.cpp)

target_include_directories(box2d_try PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${GLFW3_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer
)

target_include_directories(renderer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${GLFW3_INCLUDE_DIRS}
)

target_link_libraries(renderer box2d m imgui glfw3 OpenGL)

target_link_libraries(box2d_try box2d m imgui glfw3 OpenGL renderer)