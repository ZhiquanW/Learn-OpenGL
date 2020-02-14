project(opengl-utility)
aux_source_directory(. OpenGLApplication.cpp)
add_library(${PROJECT_NAME} STATIC OpenGLApplication.cpp)