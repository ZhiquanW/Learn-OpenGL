//
// @Author: Zhiquan Wang
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//


#ifndef HELLO_WORLD_ZWENGINE_H
#define HELLO_WORLD_ZWENGINE_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/transform.hpp>

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "Texture.h"
#include "Camera.h"


class ZWEngine {
public:
    ZWEngine();

    bool init_shader_program(const GLchar *, const GLchar *);

    bool init_engine(int, int);

    void run();

    bool attach_camera(Camera);

    Camera get_camera();

private:
    GLFWwindow *window;
    glm::vec2 window_size{};
    ShaderProgram *shader_program;
    std::map<std::string, VertexArrayObject> vao_map;

    std::vector<Texture> texture_list;
    Camera main_camera;
    GLfloat delta_time;
    GLfloat last_time;

    //functions
    bool init_window(int, int);

    static bool init_glad();

    void main_loop();

    void cleanup();

    // custom functions
    void set_render_info();

    void draw_frame();

    //utilities functions
    void process_input(GLFWwindow *);

    void add_vao(const std::string &, VertexArrayObject &);

    void activate_vao(const std::string &);

    static void disable_vao();

    void add_texture(Texture);

    void activate_texture(GLint= -1);

    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

    static void keycode_callback(GLFWwindow *, int, int, int, int);
};
void bind_vertex_buffer_object(const std::vector<GLfloat> &data,GLenum data_type = GL_STATIC_DRAW);
void bind_vertex_attribute(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const void * pointer);

#endif //HELLO_WORLD_ZWENGINE_H
