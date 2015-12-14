cmake_minimum_required(VERSION 2.8)
#add_subdirectory(case01)
project(Study)


include_directories(C:/chenchang/dev/glfw-3.1.2/include)
link_libraries(C:/chenchang/dev/glfw-3.1.2/bld/src/$<$<CONFIG:Debug>:Debug>$<$<NOT:$<CONFIG:Debug>>:Release>/glfw3.lib)


add_executable(case01 case01/main.cpp)
#target_include_directories(C:/chenchang/dev/glfw-3.1.2/bld/src/$<$<CONFIG:Debug>:Debug>$<$NOT:<$<CONFIG:Debug>>:Release>/glfw3.lib)
#target_link_libraries(C:/chenchang/dev/glfw-3.1.2/bld/src/$<$<CONFIG:Debug>:Debug>$<$NOT:<$<CONFIG:Debug>>:Release>/glfw3.lib)
