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

// Opengl functions
void ZWEngine::set_render_info() {
    glEnable(GL_DEPTH_TEST);

    // Set Render
    self = this;
    Camera main_cam;
    main_camera.set_pos(glm::vec3(0,0,6));
    this->attach_camera(main_camera);
    glfwSetFramebufferSizeCallback(this->window,framebuffer_size_callback);
    shader_program->use_shader_program();

    std::vector<GLfloat> vertices = {-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                     -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                                     -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                                     1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                     1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                     1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                                     1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f};
    std::vector<GLuint> indices = {
            0, 2, 1,
            0, 4, 1,
            0, 2, 4,
            3, 7, 1,
            3, 1, 2,
            3, 2, 7,
            6, 2, 7,
            6, 4, 7,
            6, 2, 4,
            5, 1, 7,
            5, 4, 7,
            5, 1, 4,
    };

    VertexArrayObject vao(true);
    VertexBufferObject vbo(vertices, GL_STATIC_DRAW);
    ElementBufferObject ebo(indices, GL_STATIC_DRAW);
    //pos
    bind_vertex_attribute(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) nullptr);
    //texture coord
    bind_vertex_attribute(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    //col
    bind_vertex_attribute(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (5 * sizeof(GLfloat)));
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

void ZWEngine::render_ui() {
    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Create a window called "Hello, world!" and append into it.
    ImGui::Begin("Hello, world!");
    if (this->uniform_failed_id != -1){
        std::string tmp ="uniform variable ";
        tmp+= std::to_string(uniform_failed_id);
        tmp+= " declare failed";
        ImGui::Text("%s", tmp.c_str());
    }

    ImGui::SliderFloat("obj angle: ", &obj_angle, -180.0f, 180.0f);
    ImGui::End();
    ImGui::Render();
}
void ZWEngine::render_world() {
    // clear buffers
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::rotate(glm::radians(this->obj_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    if(!shader_program->set_uniform_mat4fv(2,model)){
        this->uniform_failed_id = 2;
    }
    glm::mat4 view = this->main_camera.get_view_mat();
    if(!shader_program->set_uniform_mat4fv(3,view)){
        this->uniform_failed_id = 3;
    }
    glm::mat4 proj = this->main_camera.get_projection_mat();
    if(!shader_program->set_uniform_mat4fv(4,proj)){
        this->uniform_failed_id =4;
    }
    this->activate_texture();
    this->activate_vao("tmp_vao");
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}

void ZWEngine::process_input() {
    // check 'ESC' is pressed
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, true);
    }
}


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





