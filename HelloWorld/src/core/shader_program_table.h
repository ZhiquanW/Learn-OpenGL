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
        inline static ShaderInfo pure_shader_info{"pure", "../shaders/line_renderer.vert", "../shaders/line_renderer.frag"};
        inline static ShaderInfo blinn_phong_info{"blinn_phong_info", "../shaders/default_rendering.vert", "../shaders/blinn_phong_rendering.frag"};
        inline static ShaderInfo simple_depth_shader_info{"simple_depth", "../shaders/simple_depth_map.vert", "../shaders/simple_depth_map.frag"};

    };


}

#endif //HELLOWORLD_SHADER_PROGRAM_TABLE_H
