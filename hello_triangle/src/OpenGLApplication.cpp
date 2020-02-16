#include <glad/glad.h>

#include "../include/OpenGLApplication.h"
#include "../include/ShaderProgram.h"
#include <iostream>

// Utility Functions
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

OpenGLApplication::OpenGLApplication() {
    std::cout << "OPENGL" << std::endl;
}
// OpenGLApplication Member Functions
bool OpenGLApplication::init_application(int w, int h) {
    bool window_init = this->init_window(w, h);

    bool glad_init = this->init_glad();
    this->shader_program = new ShaderProgram();
    shader_program->attach_shader("vertex_shader", "/home/vrlab/Git-Repositories/Learn-OpenGL/hello_triangle/src/vertex_shader.glsl");
    shader_program->attach_shader("fragment_shader", "/home/vrlab/Git-Repositories/Learn-OpenGL/hello_triangle/src/fragment_shader.glsl");
    shader_program->link_program();
    this->set_render_objects();
    return window_init && glad_init;
}

void OpenGLApplication::run() {
    this->main_loop();
    this->cleanup();
}

bool OpenGLApplication::init_window(int width, int height) {
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

bool OpenGLApplication::init_glad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    }
    return true;
}

void OpenGLApplication::add_vao(unsigned int vao_id) {
    this->vao_list.push_back(vao_id);
}
void OpenGLApplication::set_render_objects() {
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindBuffer(GL_ARRAY_BUFFER, vao);
    glBindVertexArray(vao);
    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f, 0.0f, 0.5f, 0.0f};
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    this->add_vao(vao);
}
void OpenGLApplication::main_loop() {
    while (!glfwWindowShouldClose(this->window)) {
        process_input(this->window);
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program->use_shader_program();
        // std::cout << *this->vao_list.begin() << std::endl;
        glBindVertexArray(*this->vao_list.begin());
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwPollEvents(); // check for events
        glfwSwapBuffers(this->window);
    }
}

void OpenGLApplication::cleanup() {
    // optional: de-allocate all resources once they've outlived their purpose:

    glfwDestroyWindow(this->window);
    glfwTerminate();
}
