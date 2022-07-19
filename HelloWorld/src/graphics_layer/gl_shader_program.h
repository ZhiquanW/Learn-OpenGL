#pragma once

#include "common_includes.h"
#include "shader_uniform_variable.h"

namespace dawn_engine {
    class GLShaderProgram {
    private:
        std::string name = "NewShaderProgram";
        const char *vertexPath;
        const char *fragmentPath;
        unsigned int id = -1;

        void checkCompileErrors(unsigned int, std::string);

        int createShaderProgram();

    public:

        static std::vector<std::shared_ptr<ShaderUniformVariableBase>> global_uniforms;

        GLShaderProgram(const char *name, const char *vertexPath, const char *fragmentPath);

        GLShaderProgram(const char *vertexPath, const char *fragmentPath);


        ~GLShaderProgram();

        unsigned int GetID() const;

        void Activate() const;

        void Reload();

        [[nodiscard]] std::string getName() const;

        void SetBool(const std::string &name, bool value) const;

        void SetInt(const std::string &name, int value) const;

        void SetFloat(const std::string &name, float value) const;

        void SetMatrix4fv(const std::string &name, glm::mat4 value) const;

        void SetVec3fv(const std::string &name, glm::vec3 value) const;

        void SetUniform(const std::string &name, glm::vec3 value) const;

        void SetUniform(const std::string &name, bool value) const;

        void SetUniform(const std::string &name, int value) const;

        void SetUniform(const std::string &name, float value) const;

        void SetUniform(const std::string &name, glm::mat4 value) const;

        void SetUniforms(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) const;

    };

} // namespace dawn_engine