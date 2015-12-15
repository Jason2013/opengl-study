cmake_minimum_required(VERSION 2.8)

project(Study)

# https://github.com/rpavlik/cmake-modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} C:/chenchang/dev/cmake-modules)
cmake_policy(SET CMP0026 OLD)
include(CreateLaunchers)

include_directories(C:/chenchang/dev/glfw-3.1.2/include
    C:/chenchang/dev/glew-1.13.0/include
    common
)

link_libraries(opengl32.lib
    C:/chenchang/dev/glfw-3.1.2/bld/src/$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:Release>/glfw3.lib
    C:/chenchang/dev/glew-1.13.0/build/cmake/bld/lib/$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:Release>/libglew32$<$<CONFIG:Debug>:d>.lib
)

add_executable(case01 case01/main.cpp)
set_target_properties(case01 PROPERTIES
    COMPILE_DEFINITIONS GLEW_STATIC
)

add_executable(case02 case02/main.cpp
    common/shader.hpp
    common/shader.cpp
    common/globjs.h)

set_target_properties(case02 PROPERTIES
    COMPILE_DEFINITIONS GLEW_STATIC
)

create_target_launcher(case02 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/case02/")
