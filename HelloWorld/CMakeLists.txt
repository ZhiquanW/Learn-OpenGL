cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(main)
# glfw & glad
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_PROJECT_DIR ${PROJECT_SOURCE_DIR}/deps/glfw)
set(GLFW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/glfw/include)
set(GLFW_DEPS ${PROJECT_SOURCE_DIR}/deps/glfw/deps/)
add_subdirectory(${GLFW_PROJECT_DIR})
aux_source_directory(${GLFW_PROJECT_DIR}/deps  source_files)
# glm
set(GLM_SRC ${PROJECT_SOURCE_DIR}/deps/glm)
add_subdirectory(${GLM_SRC})
# bullet3
add_compile_options(-DBT_USE_DOUBLE_PRECISION)
set(BULLET_PROJECT_DIR ${PROJECT_SOURCE_DIR}/deps/bullet3)
set(BULLET_SRC_DIR ${PROJECT_SOURCE_DIR}/deps/bullet3/src)
set(BULLET_INCUDE_DIR ${PROJECT_SOURCE_DIR}/deps/bullet3/src)
add_subdirectory(${BULLET_PROJECT_DIR})
aux_source_directory(${BULLET_PROJECT_DIR}/src  source_files)
# imgui
set(IMGUI_PROJECT_DIR ${PROJECT_SOURCE_DIR}/deps/imgui)
set(IMGUI_INCLUDE_DIR 
${PROJECT_SOURCE_DIR}/deps/imgui 
${PROJECT_SOURCE_DIR}/deps/imgui/backends )
file(GLOB 
IMGUI_SRC
${PROJECT_SOURCE_DIR}/deps/imgui/*.cpp
${PROJECT_SOURCE_DIR}/deps/imgui/backends/imgui_impl_glfw.cpp
${PROJECT_SOURCE_DIR}/deps/imgui/backends/imgui_impl_opengl3.cpp
)
# aux_source_directory( source_files)

# include dir
include_directories(${GLFW_INCLUDE_DIR} ${GLFW_DEPS} ${BULLET_INCUDE_DIR} ${IMGUI_INCLUDE_DIR})

# src
aux_source_directory(src source_files)
# exe
add_executable(${PROJECT_NAME} ${source_files} ${IMGUI_SRC})
# lib
target_link_libraries(${PROJECT_NAME} glfw)


# cmake_minimum_required(VERSION 3.8)
# project(libgraphics_lib)
# set (CMAKE_CXX_STANDARD 17)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# find_package(OpenGL REQUIRED)
# find_package(OpenMP REQUIRED)

# # ImGui#add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD=1)add_definitions(-DGLM_FORCE_RADIANS)add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)#add_compile_definitions(GLM_FORCE_RADIANS=1)
# add_subdirectory(${PROJECT_SOURCE_DIR}/Dep/glm)
# add_subdirectory(${PROJECT_SOURCE_DIR}/Dep/glfw)
# add_subdirectory(${PROJECT_SOURCE_DIR}/Dep/fmt)
# add_subdirectory(${PROJECT_SOURCE_DIR}/Dep/rapidjson)
# file(GLOB render_src    
# "${PROJECT_SOURCE_DIR}/Render/*.cpp"    
# "${PROJECT_SOURCE_DIR}/Utilities/*.cpp"
# "${PROJECT_SOURCE_DIR}/Dep/tinyobjloader/*.cc"    
# "${PROJECT_SOURCE_DIR}/Dep/tinyxml2/*.cpp"    
# "${PROJECT_SOURCE_DIR}/asset_manager.cpp"    
# "${PROJECT_SOURCE_DIR}/render_engine.cpp"    
# "${PROJECT_SOURCE_DIR}/otb_window.cpp"    
# "${PROJECT_SOURCE_DIR}/Dep/glad/glad.c"    
# "${PROJECT_SOURCE_DIR}/imgui/*.cpp"    
# "${PROJECT_SOURCE_DIR}/imgui/examples/imgui_impl_glfw.cpp"    
# "${PROJECT_SOURCE_DIR}/imgui/examples/imgui_impl_opengl3.cpp")

# add_library(graphics_lib STATIC ${render_src})
# find_package(Boost COMPONENTS system filesystem REQUIRED)
# target_include_directories(graphics_lib PUBLIC 
# ${PROJECT_SOURCE_DIR}/Dep/
# ${PROJECT_SOURCE_DIR}/Dep/stb
# ${PROJECT_SOURCE_DIR}/Dep/glad
# ${PROJECT_SOURCE_DIR}/Dep/fmt/include
# ${PROJECT_SOURCE_DIR}/Dep/glfw/include
# ${PROJECT_SOURCE_DIR}/Dep/rapidjson/include
# ${PROJECT_SOURCE_DIR}/imgui
# ${PROJECT_SOURCE_DIR}
# ${Boost_INCLUDE_DIR})
# target_include_directories(graphics_lib SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/Dep/glm)
# set_target_properties(graphics_lib PROPERTIES     POSITION_INDEPENDENT_CODE ON)
# target_link_libraries(graphics_lib PUBLIC ${Boost_LIBRARIES}${OPENGL_gl_LIBRARY}glfwOpenMP::OpenMP_CXX)