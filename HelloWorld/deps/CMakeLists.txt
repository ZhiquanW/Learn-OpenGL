cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(depproj)

# ================ glfw ================
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_PROJECT_ROOT_LOCAL ${CMAKE_CURRENT_SOURCE_DIR}/glfw)
set(GLFW_INCLUDE_DIR_LOCAL ${GLFW_PROJECT_ROOT_LOCAL}/include)
# sub cmakelists
add_subdirectory(${GLFW_PROJECT_ROOT_LOCAL})
# ================ glfw ================

# ================ glad ================
set(GLAD_PROJECT_ROOT_LOCAL ${CMAKE_CURRENT_SOURCE_DIR}/glad)
set(GLAD_INCLUDE_DIR_LOCAL ${GLAD_PROJECT_ROOT_LOCAL}/include)
set(GLAD_SRC_DIR_LOCAL ${GLAD_PROJECT_ROOT_LOCAL}/src)
file(GLOB GLAD_SRC_FILES_LOCAL ${GLAD_SRC_DIR_LOCAL}/*.c)
# ================ glad ================

# ================ glm ================
set(GLM_PROJECT_ROOT_LOCAL ${CMAKE_CURRENT_SOURCE_DIR}/glm)
set(GLM_INCLUDE_DIR_LOCAL ${GLM_PROJECT_ROOT_LOCAL})
# ================ glm ================

# ================ imgui ================
set(IMGUI_PROJECT_ROOT_LOCAL ${CMAKE_CURRENT_SOURCE_DIR}/imgui )
set(IMGUI_INCLUDE_DIR_LOCAL ${IMGUI_PROJECT_ROOT_LOCAL} ${IMGUI_PROJECT_ROOT_LOCAL}/backends)
set(IMGUI_SRC_DIR_LOCAL ${IMGUI_PROJECT_ROOT_LOCAL})
file(GLOB IMGUI_SRC_FILES_LOCAL
${IMGUI_SRC_DIR_LOCAL}/*.cpp
${IMGUI_PROJECT_ROOT_LOCAL}/backends/imgui_impl_glfw.cpp
${IMGUI_PROJECT_ROOT_LOCAL}/backends/imgui_impl_opengl3.cpp)

# ================ imgui ================
set(FMT_PROJECT_ROOT_LOCAL ${CMAKE_CURRENT_SOURCE_DIR}/fmt)
set(FMT_INCLUDE_DIR_LOCAL ${FMT_PROJECT_ROOT_LOCAL}/include)
# set(FMT_SRC_DIR_LOCAL ${FMT_PROJECT_ROOT_LOCAL}/src)
add_subdirectory(${FMT_PROJECT_ROOT_LOCAL})
# ================ summarize variables ================
set(DEPS_INCLUDE_DIRS_LOCAL
${GLFW_INCLUDE_DIR_LOCAL}
${GLAD_INCLUDE_DIR_LOCAL} 
${GLM_INCLUDE_DIR_LOCAL}
${IMGUI_INCLUDE_DIR_LOCAL}
${FMT_INCLUDE_DIR_LOCAL})

set(DEPS_SRC_FILES_LOCAL
${GLAD_SRC_FILES_LOCAL}
${IMGUI_SRC_FILES_LOCAL})

add_library(deps_shared_lib SHARED 
${DEPS_SRC_FILES_LOCAL})

target_include_directories(deps_shared_lib PUBLIC  ${DEPS_INCLUDE_DIRS_LOCAL})
target_link_libraries(deps_shared_lib PUBLIC glfw fmt::fmt)