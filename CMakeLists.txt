cmake_minimum_required(VERSION 2.8)
#add_subdirectory(case01)
project(Study)


include_directories(C:/chenchang/dev/glfw-3.1.2/include
    C:/chenchang/dev/glew-1.13.0/include
)

link_libraries(opengl32.lib
    C:/chenchang/dev/glfw-3.1.2/bld/src/$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:Release>/glfw3.lib
    C:/chenchang/dev/glew-1.13.0/build/cmake/bld/lib/$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:Release>/libglew32$<$<CONFIG:Debug>:d>.lib
)


add_executable(case01 case01/main.cpp)
target_compile_definitions(case01 PUBLIC -DGLEW_STATIC)
