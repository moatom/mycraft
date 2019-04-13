cmake_minimum_required(VERSION 3.13)

project(mycraft CXX)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -O3 -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

if(APPLE)
#   find_library(COCOA Cocoa REQUIRED)
  find_library(CoreFoundation CoreFoundation REQUIERD)
    # if (NOT COCOA)
    #     message(FATAL_ERROR "Cocoa not found")
    # endif()
ENDIF(APPLE)

include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLM_INCLUDE_DIRS} include)
link_directories(lib)

#file(GLOB_RECURSE SRC_FILES "*.cpp")
#add_executable(mycraft ${SRC_FILES})
add_executable(mycraft main.cpp model.cpp)

# target_link_libraries(mycraft ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} glfw ${GLM_LIBRARY_DIRS} m SOIL ${COCOA_LIBRARY})
target_link_libraries(mycraft ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} glfw ${GLM_LIBRARY_DIRS} m SOIL ${CoreFoundation})
