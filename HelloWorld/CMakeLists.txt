cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(HelloWorld)
# set variables
set(PROJECT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# execute subdirectory cmakelists
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/modules)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/utils)
# include
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${MODULES_DIR}
        ${CORE_DIR}
        ${UTILS_DIR}
)
# src
file(GLOB_RECURSE
        SOURCE_FILES
        ${PROJECT_SRC_DIR}/*.cpp
        )
# exe
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
# lib
target_link_libraries(${PROJECT_NAME} deps_shared_lib)
