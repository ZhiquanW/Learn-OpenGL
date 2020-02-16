#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <vector>
class ShaderProgram {
  private:
    int success_flag;
    static const short loginfo_len = 512;
    char log_info[loginfo_len];
    unsigned int shader_program_id;
    std::vector<unsigned int> shader_id_list;

  public:
    ShaderProgram();
    void use_shader_program();
    void attach_shader(const char *, const GLchar *);
    void link_program();
};

#endif