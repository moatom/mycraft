cmake_minimum_required(VERSION 2.8)

project(mycraft CXX)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -O3 -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLM_INCLUDE_DIRS})

#file(GLOB_RECURSE SRC_FILES "*.cpp")
#add_executable(mycraft ${SRC_FILES})
add_executable(mycraft main.cpp model.cpp)

target_link_libraries(mycraft ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} glfw ${GLM_LIBRARY_DIRS} m)
