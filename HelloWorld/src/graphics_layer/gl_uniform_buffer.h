//
// Created by zhiquan on 7/1/22.
//

#ifndef HELLOWORLD_GL_UNIFORM_BUFFER_H
#define HELLOWORLD_GL_UNIFORM_BUFFER_H

#include "common_includes.h"
#include "shader_uniform_variable.h"
#include "gl_shader_program.h"

namespace dawn_engine {

    class GLUniformBuffer {
    private:
    protected:
        unsigned int ubo_ = 0;
        long buffer_size_ = 0;
        unsigned int binding_idx_ = 0;
        std::vector<std::pair<unsigned int, std::shared_ptr<ShaderUniformVariableBase>>> uniform_data_sequence;
    public:

        GLUniformBuffer(unsigned int binding_idx);

        GLUniformBuffer(unsigned int binding_idx, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);


        void GenBuffer();

        void RefreshData(unsigned int begin_idx, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);

        void SyncGLSubData(int begin_idx, int end_idx);

        void SyncGLData();


        void AppendUniform(const std::shared_ptr<ShaderUniformVariableBase> &uniform);

        void AppendUniforms(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);

        void AppendStructUniforms(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);

        [[nodiscard]] unsigned int GetUBO() const;
    };

}

#endif //HELLOWORLD_GL_UNIFORM_BUFFER_H
