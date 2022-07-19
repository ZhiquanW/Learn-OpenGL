//
// Created by zhiquan on 6/23/22.
//

#ifndef HELLOWORLD_GL_RENDER_OBJECT_H
#define HELLOWORLD_GL_RENDER_OBJECT_H

#include "common_includes.h"
#include "core/dawn_vertex.h"
#include "shader_uniform_variable.h"
#include "core/dawn_material.h"
#include "core/dawn_mesh.h"

namespace dawn_engine {

    enum GLRenderElement {
        TRIANGLE, LINE
    };


    class  GLRenderObject {
    private:
        GLRenderElement render_element_ = GLRenderElement::TRIANGLE;
        unsigned int vao_ = 0;
        unsigned int vbo_ = 0;
        unsigned int ebo_ = 0;
        unsigned int indices_num_ = 0;
        std::vector<unsigned int> diffuse_tex_ids_ = {};
        std::vector<unsigned int> specular_tex_ids_ = {};
        std::vector<unsigned int> normal_tex_ids_ = {};
        unsigned int cube_map_tex_id_ = 0;
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms_ = {};
        GLShaderProgram *linked_shader_ = nullptr;

        void BindGLData() const;

        void UnbindGLData() const;

    public:
        GLRenderObject() = default;

        explicit GLRenderObject(unsigned int vao,
                                unsigned int vbo,
                                unsigned int ebo,
                                unsigned int indices_num,
                                const ShaderInfo &shader_info);

        explicit GLRenderObject(GLRenderElement render_element,
                                unsigned int vao,
                                unsigned int vbo,
                                unsigned int ebo,
                                unsigned int indices_num,
                                const ShaderInfo &shader_info);

        explicit GLRenderObject(unsigned int vao,
                                unsigned int vbo,
                                unsigned int ebo,
                                unsigned int indices_num,
                                std::vector<unsigned int> diffuse_tex_ids,
                                std::vector<unsigned int> specular_tex_ids,
                                std::vector<unsigned int> normal_tex_ids,
                                const ShaderInfo &shader_info);

        explicit GLRenderObject(unsigned int vao,
                                unsigned int vbo,
                                unsigned int ebo,
                                unsigned int indices_num,
                                unsigned int cube_map_tex_id,
                                const ShaderInfo &shader_info);

        ~GLRenderObject();


        void ReleaseGLData();

        void RefreshUniforms(std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms);

        void RefreshShaderProgram(GLShaderProgram * shader_program);

        void render() const;

//        DawnMesh *GetLinkedMesh() const;
    };
}

#endif //HELLOWORLD_GL_RENDER_OBJECT_H
