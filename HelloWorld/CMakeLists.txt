cmake_minimum_required(VERSION 3.10)
project(main)
include_directories(${CMAKE_SOURCE_DIR}/includes ${CMAKE_SOURCE_DIR}/dependency/)
link_directories(${CMAKE_SOURCE_DIR}/dependency/GLFW)

aux_source_directory(${CMAKE_SOURCE_DIR}/src source_files)

add_executable(${PROJECT_NAME} ${source_files})
target_link_libraries(${PROJECT_NAME} glfw3 GL)

