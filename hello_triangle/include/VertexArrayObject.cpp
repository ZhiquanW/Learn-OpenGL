#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &this->vao_id);
}

void VertexArrayObject::activate() {
    glBindVertexArray(this->vao_id);
}

void VertexArrayObject::deactive() {
    glBindVertexArray(this->vao_id);
}
void VertexArrayObject::add_vertex_buffer_object(std::vector<glm::vec3> data,
                                                 GLuint index,
                                                 GLsizei size,
                                                 const void *offset) {
    //data, location,size,stride,offset
    unsigned int vbo_id;
    glGenBuffers(q, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
}
