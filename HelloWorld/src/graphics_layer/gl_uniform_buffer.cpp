//
// Created by zhiquan on 7/1/22.
//

#include "gl_uniform_buffer.h"

namespace dawn_engine {

    GLUniformBuffer::GLUniformBuffer(unsigned int binding_idx, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) : binding_idx_(binding_idx) {
        auto uniforms_sizes = ShaderUniformVariableBase::GetUniformsDataSizes(uniforms);
        this->AppendUniforms(uniforms);
        this->GenBuffer();
        for (int i = 0; i < this->uniform_data_sequence.size(); ++i) {
            this->RefreshData(0, uniforms);
        }
    }

    GLUniformBuffer::GLUniformBuffer(unsigned int binding_idx) : binding_idx_(binding_idx) {

    }

    void GLUniformBuffer::GenBuffer() {
        glGenBuffers(1, &this->ubo_);
        glBindBuffer(GL_UNIFORM_BUFFER, this->ubo_);
        glBufferData(GL_UNIFORM_BUFFER, this->buffer_size_, nullptr, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, this->binding_idx_, this->ubo_, 0, this->buffer_size_);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    unsigned int GLUniformBuffer::GetUBO() const {
        return this->ubo_;
    }

    void GLUniformBuffer::RefreshData(unsigned int begin_idx, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        int uniform_num = uniforms.size();
        for (int i = 0; i < uniform_num; ++i) {
            if (uniforms[i]->GetTypeHash() != this->uniform_data_sequence[i + begin_idx].second->GetTypeHash()) {
                throw std::runtime_error("refresh of different shader uniform variable type");
            }
            this->uniform_data_sequence[i + begin_idx].second = uniforms[i];
        }
    }

    void GLUniformBuffer::SyncGLSubData(int begin_idx, int end_idx) {// inclusive, exclusive
        begin_idx = std::max(0, begin_idx);
        if (end_idx < 1) {
            end_idx = this->uniform_data_sequence.size();
        }
        end_idx = std::min((int) this->uniform_data_sequence.size(), end_idx);
        if (end_idx < 1) {
            return;
        }
        glBindBuffer(GL_UNIFORM_BUFFER, this->ubo_);
        for (int i = begin_idx; i < end_idx; ++i) {
            if (this->uniform_data_sequence[i].second->GetTypeHash() == typeid(int).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, this->uniform_data_sequence[i].first, sizeof(int),
                                &std::dynamic_pointer_cast<ShaderUniformVariable<int>>(this->uniform_data_sequence[i].second)->GetDataRef());
            } else if (this->uniform_data_sequence[i].second->GetTypeHash() == typeid(bool).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, this->uniform_data_sequence[i].first, sizeof(int),
                                &std::dynamic_pointer_cast<ShaderUniformVariable<bool>>(this->uniform_data_sequence[i].second)->GetDataRef());
            } else if (this->uniform_data_sequence[i].second->GetTypeHash() == typeid(float).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, this->uniform_data_sequence[i].first, sizeof(float),
                                &std::dynamic_pointer_cast<ShaderUniformVariable<float>>(this->uniform_data_sequence[i].second)->GetDataRef());
            } else if (this->uniform_data_sequence[i].second->GetTypeHash() == typeid(glm::vec3).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, this->uniform_data_sequence[i].first, sizeof(glm::vec3),
                                glm::value_ptr(std::dynamic_pointer_cast<ShaderUniformVariable<glm::vec3>>(this->uniform_data_sequence[i].second)->GetDataRef()));
            } else if (this->uniform_data_sequence[i].second->GetTypeHash() == typeid(glm::mat4).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, this->uniform_data_sequence[i].first, sizeof(glm::mat4),
                                glm::value_ptr(std::dynamic_pointer_cast<ShaderUniformVariable<glm::mat4>>(this->uniform_data_sequence[i].second)->GetDataRef()));
            } else {
                throw std::runtime_error("unknown shader uniform variable type");
            }
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void GLUniformBuffer::SyncGLData() {
        glBindBuffer(GL_UNIFORM_BUFFER, this->ubo_);
        for (const auto &data_pair: this->uniform_data_sequence) {
            if (data_pair.second->GetTypeHash() == typeid(int).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, data_pair.first, sizeof(int),
                                &std::dynamic_pointer_cast<ShaderUniformVariable<int>>(data_pair.second)->GetDataRef());
            } else if (data_pair.second->GetTypeHash() == typeid(bool).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, data_pair.first, sizeof(bool),
                                &std::dynamic_pointer_cast<ShaderUniformVariable<bool>>(data_pair.second)->GetDataRef());
            } else if (data_pair.second->GetTypeHash() == typeid(float).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, data_pair.first, sizeof(float),
                                &std::dynamic_pointer_cast<ShaderUniformVariable<float>>(data_pair.second)->GetDataRef());
            } else if (data_pair.second->GetTypeHash() == typeid(glm::vec3).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, data_pair.first, sizeof(glm::vec3),
                                glm::value_ptr(std::dynamic_pointer_cast<ShaderUniformVariable<glm::vec3>>(data_pair.second)->GetDataRef()));
            } else if (data_pair.second->GetTypeHash() == typeid(glm::mat4).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, data_pair.first, sizeof(glm::mat4),
                                glm::value_ptr(std::dynamic_pointer_cast<ShaderUniformVariable<glm::mat4>>(data_pair.second)->GetDataRef()));
            } else {
                throw std::runtime_error("unknown shader uniform variable type");
            }
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void GLUniformBuffer::AppendUniform(const std::shared_ptr<ShaderUniformVariableBase> &uniform) {
        // align buffer offset with 16 bytes
        unsigned int base_alignment = ShaderUniformVariableBase::GetDataBaseAlignment({uniform});
        unsigned int next_align_reminder = this->buffer_size_ % base_alignment;
        if (next_align_reminder > 0) {
            this->buffer_size_ = (buffer_size_ / base_alignment + 1) * base_alignment;
        }
        this->uniform_data_sequence.emplace_back(this->buffer_size_, uniform);
        this->buffer_size_ += ShaderUniformVariableBase::GetUniformDataSize(uniform);
    }

    void GLUniformBuffer::AppendUniforms(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        for (const auto &uniform: uniforms) {
            this->AppendUniform(uniform);
        }

    }

    void GLUniformBuffer::AppendStructUniforms(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        unsigned int base_alignment = 16;
        unsigned int next_align_reminder = this->buffer_size_ % base_alignment;
        if (next_align_reminder > 0) {
            this->buffer_size_ = (buffer_size_ / base_alignment + 1) * base_alignment;
        }
        for (const auto &uniform: uniforms) {
            this->AppendUniform(uniform);
        }
    }


}
