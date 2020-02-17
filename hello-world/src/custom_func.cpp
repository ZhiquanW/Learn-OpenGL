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


// Opengl functions
void ZWEngine::set_render_objects() {
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
    bind_vertex_attribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) nullptr);
    bind_vertex_attribute(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    bind_vertex_attribute(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));
    vao.attach_vbo(vbo.id());
    vao.attach_ebo(ebo.id());
    this->add_vao("tmp_vao", vao);
    ZWEngine::disable_vao();

    Texture tex_0(1);
    tex_0.load_image("../resources/test0.jpeg");
    Texture tex_1(2);
    tex_1.load_image("../resources/test_image.jpg");
    this->add_texture(tex_0);
    this->add_texture(tex_1);
}
void ZWEngine::draw_frame() {
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, this->texture1);
//    glUniform1i(5,0);
//    glBindVertexArray(1);
    this->activate_texture();
    this->activate_vao("tmp_vao");
//    glDrawArrays(GL_TRIANGLES,0,3);
    glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0);
}

void ZWEngine::process_input(GLFWwindow *window) {
    // check 'ESC' is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}