//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include "ZWEngine.h"
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include <iostream>

// Utility Functions
void release_vao(){
    glBindVertexArray(0);
}
void framebuffer_size_callback(GLFWwindow, int, int);
void process_input(GLFWwindow *);

void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
}

void process_input(GLFWwindow *window) {
    // check 'ESC' is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
// Opengl functions
void bind_vertex_buffer_object(const std::vector<GLfloat> &data,GLenum data_type = GL_STATIC_DRAW){
    GLuint vbo_id = -1;
    glGenBuffers(1,&vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_id);
    glBufferData(GL_ARRAY_BUFFER,data.size() * sizeof(GLfloat),&data[0],data_type);
}
void bind_vertex_attribute(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const void * pointer){
    glVertexAttribPointer(index,size,type,normalized,stride,pointer);
    glEnableVertexAttribArray(index);
}
ZWEngine::ZWEngine() {
    this->window = nullptr;
    this->shader_program = nullptr;
    std::cout << "initialing Opengl engine ..." << std::endl;
}
// ZWEngine Member Functions
bool ZWEngine::init_engine(int w, int h) {
    bool window_init = this->init_window(w, h);

    bool glad_init = ZWEngine::init_glad();
    this->shader_program = new ShaderProgram();
    shader_program->attach_shader("vertex_shader", "/home/vrlab/Git-Repositories/Learn-OpenGL/hello_triangle/src/vertex_shader.glsl");
    shader_program->attach_shader("fragment_shader", "/home/vrlab/Git-Repositories/Learn-OpenGL/hello_triangle/src/fragment_shader.glsl");
    shader_program->link_program();
    this->set_render_objects();
    return window_init && glad_init;
}

void ZWEngine::run() {
    this->main_loop();
    this->cleanup();
}

void ZWEngine::set_render_objects() {
    std::vector<GLfloat> vertices = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                                     0.0f, 0.0f, 0.5f, 0.0f};
    VertexArrayObject vao(true);

    bind_vertex_buffer_object(vertices,GL_STATIC_DRAW);
    bind_vertex_attribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) nullptr);
    release_vao();
    this->add_vao("tmp_vao", vao);

}
void ZWEngine::draw_frame() {
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_program->use_shader_program();
//    glBindVertexArray(1);
        this->activate_vao("tmp_vao");

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ZWEngine::activate_vao(const std::string& name) {
    glBindVertexArray(this->vao_map.at(name).id());

}
// private member functions
bool ZWEngine::init_window(int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // params: width, height, title, monitor, opengl related}
    this->window = glfwCreateWindow(width, height, "opengl", nullptr, nullptr);
    if (this->window == nullptr) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);

    return true;
}

bool ZWEngine::init_glad() {
    return gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 0;
}

void ZWEngine::add_vao(const std::string &name, VertexArrayObject &vao) {
    this->vao_map.insert(std::pair<std::string, VertexArrayObject> ("tmp_vao",vao));
}

void ZWEngine::main_loop() {
    while (!glfwWindowShouldClose(this->window)) {
        process_input(this->window);
        this->draw_frame();
        glfwPollEvents(); // check for events
        glfwSwapBuffers(this->window);
    }
}

void ZWEngine::clean_vao(){
    std::map<std::string ,GLuint>::iterator itor;
    for(utir)
}
void ZWEngine::cleanup() {
    // optional: de-allocate all resources once they've outlived their purpose:

    glfwDestroyWindow(this->window);
    glfwTerminate();
}
