#include "shader_uniform_variable.h"

namespace dawn_engine {
    std::string ShaderUniformVariableBase::GetName() const { return this->name; }

    std::size_t ShaderUniformVariableBase::GetTypeHash() const { return this->type_hash; }

    std::vector<long> ShaderUniformVariableBase::GetUniformsDataSizes(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        std::vector<long> data_sizes = {};
        for (const auto &u_sp: uniforms) {
            ShaderUniformVariableBase *u_ptr = u_sp.get();
            if (u_ptr->GetTypeHash() == typeid(int).hash_code()) {
                data_sizes.push_back(dynamic_cast<ShaderUniformVariable<int> *>(u_ptr)->GetDataSize());
            } else if (u_ptr->GetTypeHash() == typeid(bool).hash_code()) {
                data_sizes.push_back(dynamic_cast<ShaderUniformVariable<int> *>(u_ptr)->GetDataSize());
            } else if (u_ptr->GetTypeHash() == typeid(float).hash_code()) {
                data_sizes.push_back(dynamic_cast<ShaderUniformVariable<float> *>(u_ptr)->GetDataSize());
            } else if (u_ptr->GetTypeHash() == typeid(glm::vec3).hash_code()) {
                data_sizes.push_back(dynamic_cast<ShaderUniformVariable<glm::vec3> *>(u_ptr)->GetDataSize());
            } else if (u_ptr->GetTypeHash() == typeid(glm::mat4).hash_code()) {
                data_sizes.push_back(dynamic_cast<ShaderUniformVariable<glm::mat4> *>(u_ptr)->GetDataSize());
            } else {
                throw std::runtime_error("unknown shader uniform variable type");
            }
        }
        return data_sizes;
    }

    long ShaderUniformVariableBase::GetUniformsDataSize(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        auto data_sizes = ShaderUniformVariableBase::GetUniformsDataSizes(uniforms);
        return std::accumulate(data_sizes.begin(), data_sizes.end(), 0);
    }

    long ShaderUniformVariableBase::GetUniformDataSize(const std::shared_ptr<ShaderUniformVariableBase> uniform) {
        if (uniform->GetTypeHash() == typeid(int).hash_code()) {
            return std::dynamic_pointer_cast<ShaderUniformVariable<int>>(uniform)->GetDataSize();
        } else if (uniform->GetTypeHash() == typeid(bool).hash_code()) {
            return std::dynamic_pointer_cast<ShaderUniformVariable<bool>>(uniform)->GetDataSize();
        } else if (uniform->GetTypeHash() == typeid(float).hash_code()) {
            return std::dynamic_pointer_cast<ShaderUniformVariable<float>>(uniform)->GetDataSize();
        } else if (uniform->GetTypeHash() == typeid(glm::vec3).hash_code()) {
            return std::dynamic_pointer_cast<ShaderUniformVariable<glm::vec3>>(uniform)->GetDataSize();
        } else if (uniform->GetTypeHash() == typeid(glm::mat4).hash_code()) {
            return std::dynamic_pointer_cast<ShaderUniformVariable<glm::mat4>>(uniform)->GetDataSize();
        } else {
            throw std::runtime_error("unknown shader uniform variable type");
        }
    }

    long ShaderUniformVariableBase::GetDataBaseAlignment(const std::shared_ptr<ShaderUniformVariableBase> &uniform) {
        if (uniform->GetTypeHash() == typeid(int).hash_code()) {
            return 4;
        } else if (uniform->GetTypeHash() == typeid(bool).hash_code()) {
            return 4;
        } else if (uniform->GetTypeHash() == typeid(float).hash_code()) {
            return 4;
        } else if (uniform->GetTypeHash() == typeid(glm::vec3).hash_code()) {
            return 16;
        } else if (uniform->GetTypeHash() == typeid(glm::mat4).hash_code()) {
            return 64;
        } else {
            throw std::runtime_error("unknown shader uniform variable type");
        }
    }
} // namespace dawn_engine