#pragma once

#include "common_includes.h"
#include "shader_uniform_variable.h"

namespace dawn_engine {
    class OpenGLShaderProgram {
    private:
        std::string name = "NewShaderProgram";
        const char *vertexPath;
        const char *fragmentPath;
        unsigned int id = -1;

        void checkCompileErrors(unsigned int, std::string);

        int createShaderProgram();

    public:
        OpenGLShaderProgram(const char *name, const char *vertexPath, const char *fragmentPath);

        OpenGLShaderProgram(const char *vertexPath, const char *fragmentPath);


        ~OpenGLShaderProgram();

        void activate() const;

        void reload();

        [[nodiscard]] std::string getName() const;

        void setBool(const std::string &name, bool value) const;

        void setInt(const std::string &name, int value) const;

        void setFloat(const std::string &name, float value) const;

        void setMatrix4fv(const std::string &name, glm::mat4 value) const;

        void setVec3fv(const std::string &name, glm::vec3 value) const;

        void setUniform(const std::string &, glm::vec3 value) const;

        void setUniform(const std::string &, bool value) const;

        void setUniform(const std::string &, int value) const;

        void setUniform(const std::string &, float value) const;

        void setUniform(const std::string &, glm::mat4 value) const;

        void setUniforms(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &) const;
    };

} // namespace dawn_engine