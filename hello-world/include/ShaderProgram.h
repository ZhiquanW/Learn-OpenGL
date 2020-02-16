//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#ifndef HELLO_WORLD_A_H
#define HELLO_WORLD_A_H

#include <glad/glad.h>
#include <vector>
class ShaderProgram {
private:
    int success_flag{};
    static const short loginfo_len = 512;
    char log_info[loginfo_len]{};
    unsigned int shader_program_id;
    std::vector<unsigned int> shader_id_list;

public:
    ShaderProgram();
    void use_shader_program();
    void attach_shader(const char *, const GLchar *);
    void link_program();
};



#endif //HELLO_WORLD_A_H
