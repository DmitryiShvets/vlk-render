cmake_minimum_required (VERSION 3.12)
set(PROJECT_NAME VLK_TEST)
project(${PROJECT_NAME})

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(IMGUI_PATH ${CMAKE_SOURCE_DIR}/imgui)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(test1)
add_subdirectory(test2)
