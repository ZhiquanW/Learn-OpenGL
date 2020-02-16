#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class VertexArrayObject {
  public:
    VertexArrayObject();
    void activate();
    void deactive();
    void add_vertex_buffer_object(std::vector<glm::vec3>, GLuint, GLsizei, const void *); //data, location,size,stride,offset
    void add_element_buffer_object();

  private:
    unsigned int vao_id;
    std::vector<unsigned int> vbo_id_list;
    std::vector<unsigned int> veo_id_list;
};

class VertexAttribute {
};
#endif