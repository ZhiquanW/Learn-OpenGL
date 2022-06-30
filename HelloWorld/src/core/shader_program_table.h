//
// Created by zhiquan on 6/24/22.
//

#ifndef HELLOWORLD_SHADER_PROGRAM_TABLE_H
#define HELLOWORLD_SHADER_PROGRAM_TABLE_H

#include "common_includes.h"

namespace dawn_engine {
    struct ShaderInfo {
        std::string name;
        std::string vert_path;
        std::string frag_path;
    };

    class ShaderTable {
    public:
        inline static ShaderInfo default_shader_info{"default", "../shaders/default_rendering.vert", "../shaders/default_rendering.frag"};
        inline static ShaderInfo depth_shader_info{"depth", "../shaders/default_rendering.vert", "../shaders/depth_buffer_rendering.frag"};
        inline static ShaderInfo skybox_shader_info{"skybox", "../shaders/skybox.vert", "../shaders/skybox.frag"};

    };


}

#endif //HELLOWORLD_SHADER_PROGRAM_TABLE_H