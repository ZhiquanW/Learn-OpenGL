#include <glad/glad.h>

#include "OpenGLApplication.h"
#include <iostream>
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

bool OpenGLApplication::init_application(int w, int h) {
  bool window_init = this->init_window(w, h);
  bool glad_init = this->init_glad();
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
void OpenGLApplication::main_loop() {
  while (!glfwWindowShouldClose(this->window)) {
    process_input(this->window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents(); // check for events
    glfwSwapBuffers(this->window);
  }
}

void OpenGLApplication::cleanup() {
  glfwDestroyWindow(this->window);
  glfwTerminate();
}
