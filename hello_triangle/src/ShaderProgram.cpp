#include "../include/ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
ShaderProgram::ShaderProgram() {
    std::cout << "INIT" << std::endl;
    this->shader_program_id = glCreateProgram();
}

void ShaderProgram::attach_shader(const GLchar *type, const GLchar *file_path) {
    // read vertex shader from file
    std::string shader_code_str;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shader_file.open(file_path);
        std::stringstream file_stream;
        file_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code_str = file_stream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCUSSFULLY_READ" << std::endl;
    }
    const char *shader_code_char = shader_code_str.c_str();
    // compile shader code
    unsigned int shader_id;
    std::cout << type << std::endl;
    if (strcmp(type, "vertex_shader") == 0) {
        shader_id = glCreateShader(GL_VERTEX_SHADER);
        std::cout << " vertex shader" << std::endl;
    } else if (strcmp(type, "fragment_shader") == 0) {
        shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    }
    glShaderSource(shader_id, 1, &shader_code_char, NULL);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &(this->success_flag));
    if (!this->success_flag) {
        glGetShaderInfoLog(shader_id, 512, NULL, this->log_info);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << this->log_info << std::endl;
    }
    glAttachShader(this->shader_program_id, shader_id);
    this->shader_id_list.push_back(shader_id);
}

void ShaderProgram::link_program() {
    std::cout << "CC" << std::endl;
    glLinkProgram(this->shader_program_id);
    glGetProgramiv(this->shader_program_id, GL_LINK_STATUS, &this->success_flag);
    if (!this->success_flag) {
        glGetProgramInfoLog(this->shader_program_id, 512, NULL, this->log_info);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << this->log_info << std::endl;
    }
    //Delete shader as it has been linked to the shader programs
    std::vector<unsigned int>::iterator itor;
    for (itor = this->shader_id_list.begin(); itor != this->shader_id_list.end(); ++itor) {
        glDeleteShader(*itor);
    }
    this->shader_id_list.clear();
}

void ShaderProgram::use_shader_program() {
    glUseProgram(this->shader_program_id);
}