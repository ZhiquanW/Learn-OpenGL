cmake_minimum_required(VERSION 3.12)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 14)

include_directories(./include)
add_subdirectory(./include)
#　添加源文件
set(SOURCE_FILES src/main.cpp src/glad.c include/OpenGLApplication.cpp include/ShaderProgram.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glfw3 GL Xi X11 pthread dl )
