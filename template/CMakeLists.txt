cmake_minimum_required(VERSION 3.12)
project(opengl_template)

set(CMAKE_CXX_STANDARD 14)

#　添加源文件
set(SOURCE_FILES src/main.cpp src/glad.c)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glfw3 GL Xi X11 pthread dl )
