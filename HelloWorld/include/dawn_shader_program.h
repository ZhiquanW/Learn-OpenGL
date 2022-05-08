#pragma once
#include "common_includes.h"
#include "shader_uniform_variable.h"
namespace dawn_engine {
class DawnShaderProgram {
  private:
    unsigned int id;
    void checkCompileErrors(unsigned int, std::string);

  public:
    DawnShaderProgram(const char *vertexPath, const char *fragmentPath);
    ~DawnShaderProgram();

    void activate();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMatrix4fv(const std::string &name, glm::mat4 value) const;
    void setVec3fv(const std::string &name, glm::vec3 value) const;
    void setUniform(const std::string &, glm::vec3 value);
    void setUniform(const std::string &, bool value);
    void setUniform(const std::string &, int value);
    void setUniform(const std::string &, glm::mat4 value);
    void setUniforms(std::vector<std::shared_ptr<ShaderUniformVariableBase>>);
};

} // namespace dawn_engine