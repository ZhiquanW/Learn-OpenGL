//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef HELLO_WORLD_ZWENGINE_H
#define HELLO_WORLD_ZWENGINE_H

#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include <GLFW/glfw3.h>
#include <map>
#include <string>
class ZWEngine {
public:
    ZWEngine();
    void run();
    bool init_engine(int, int);
private:

    GLFWwindow *window;
    ShaderProgram *shader_program;
    std::map<std::string,VertexArrayObject> vao_map;
    bool init_window(int, int);
    static bool init_glad();
    void main_loop();
    void cleanup();
    void set_render_objects();
    void draw_frame();
    void activate_vao(const std::string& );
    void add_vao(const std::string &,VertexArrayObject &);
};

#endif //HELLO_WORLD_ZWENGINE_H
