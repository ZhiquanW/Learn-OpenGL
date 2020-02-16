//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef HELLO_WORLD_VERTEXBUFFEROBJECT_H
#define HELLO_WORLD_VERTEXBUFFEROBJECT_H

#include <glad/glad.h>
#include <vector>
class VertexAttribute{
private:
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const void * pointer;
public:
    VertexAttribute();
    VertexAttribute(GLuint ,GLint ,GLenum,GLboolean,GLsizei,const void *);
    void activate();
};
class VertexBufferObject {
private:
    unsigned  int vbo_id;
public:
    VertexBufferObject( const std::vector<GLfloat> & ,GLenum);
};


#endif //HELLO_WORLD_VERTEXBUFFEROBJECT_H
