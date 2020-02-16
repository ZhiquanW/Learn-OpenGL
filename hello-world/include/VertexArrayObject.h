//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef HELLO_WORLD_VERTEXARRAYOBJECT_H
#define HELLO_WORLD_VERTEXARRAYOBJECT_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class VertexArrayObject {
private:
    GLuint vao_id;
public:
    VertexArrayObject();
    explicit VertexArrayObject(bool);
    GLuint id();
    void activate();
    void attach_vbo(const GLuint * ); //data, location,size,stride,offset
    void attach_ebo(const GLuint *);

};


#endif //HELLO_WORLD_VERTEXARRAYOBJECT_H
