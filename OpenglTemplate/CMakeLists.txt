cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(main)
# execute subdirectory cmakelists
add_subdirectory(${PROJECT_SOURCE_DIR}/deps)
# src
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE_DIR)
# exe
add_executable(${PROJECT_NAME} ${SOURCE_DIR})
# lib
target_link_libraries(${PROJECT_NAME} deps_shared_lib)
