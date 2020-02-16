cmake_minimum_required(VERSION 3.10)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
#add_subdirectory(include)

#　
aux_source_directory(src source_files)
add_executable(${PROJECT_NAME} ${source_files})
target_link_libraries(${PROJECT_NAME} glfw3 GL Xi X11 pthread dl )