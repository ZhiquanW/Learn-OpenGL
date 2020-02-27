//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//
#include "ZWEngine.h"

//#define STB_IMAGE_IMPLEMENTATION
//
//#include "stb_image.h"
static ZWEngine *self;

void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
}

bool first_in = true;
glm::vec2 pre_mouse_pos;
// callback function


void ZWEngine::keycode_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        self->main_camera.process_keyboard(FORWARD, self->delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        self->main_camera.process_keyboard(BACKWARD, self->delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        self->main_camera.process_keyboard(LEFT, self->delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        self->main_camera.process_keyboard(RIGHT, self->delta_time);
}


void ZWEngine::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (first_in) {
        pre_mouse_pos = glm::vec2(xpos, ypos);
        first_in = false;
    }
    glm::vec2 offset(xpos - pre_mouse_pos.x, pre_mouse_pos.y - ypos);
    pre_mouse_pos = glm::vec2(xpos, ypos);
    self->main_camera.process_mouse_movement(offset);
}

void ZWEngine::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    self->main_camera.process_mouse_scroll(yoffset);
}

// Opengl functions
void ZWEngine::set_render_info() {
//    self = this;
//    glfwSetFramebufferSizeCallback(this->window,framebuffer_size_callback);
    shader_program->use_shader_program();

    std::vector<GLfloat> vertices = {0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                                     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                                     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    std::vector<GLuint> indices = {
            0, 1, 3,
            1, 2, 3
    };

    VertexArrayObject vao(true);
    VertexBufferObject vbo(vertices, GL_STATIC_DRAW);
    ElementBufferObject ebo(indices, GL_STATIC_DRAW);
    //pos
    bind_vertex_attribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) nullptr);
    //col
    bind_vertex_attribute(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    //texture coord
    bind_vertex_attribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));
    vao.attach_vbo(vbo.id());
    vao.attach_ebo(ebo.id());
    this->add_vao("tmp_vao", vao);
    ZWEngine::disable_vao();

    Texture tex_0(0);
    tex_0.load_image("../resources/test0.jpeg");
    this->add_texture(tex_0);
    Texture tex_1(1);
    tex_1.load_image("../resources/test_image.jpg");
    this->add_texture(tex_1);
}

void ZWEngine::draw_frame() {

    // clear buffers
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, this->texture1);
//    glUniform1i(5,0);
//    glBindVertexArray(1);
    glm::mat4 view = this->main_camera.get_view_mat();
    bool tmp = shader_program->set_uniform_mat4fv(2,view);
    glm::mat4 proj = glm::perspective(100.0f,this->window_size.x / this->window_size.y,0.001f,10000.0f);
    tmp = shader_program->set_uniform_mat4fv(3,proj);
//    std::cout << tmp<< std::endl;
    this->activate_texture();
    this->activate_vao("tmp_vao");
//    glDrawArrays(GL_TRIANGLES,0,3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ZWEngine::process_input(GLFWwindow *window) {
    // check 'ESC' is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}