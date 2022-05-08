#pragma once
#include "base_module.h"

#include "common_includes.h"
#include "shader_uniform_variable.h"
namespace dawn_engine {
class LightModule : public BaseModule {
  private:
  protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

  public:
    LightModule();
    LightModule(glm::vec3 color);
    LightModule(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    virtual ~LightModule() = 0;

    glm::vec3 getAmbient() const;
    void setAmbient(glm::vec3 value);
    glm::vec3 getDiffuse() const;
    void setDiffuse(glm::vec3 value);
    glm::vec3 getSpecular() const;
    void setSpecular(glm::vec3 value);
    float getShininess() const;
    void setShinibess(float value);
    virtual std::vector<std::shared_ptr<ShaderUniformVariableBase>> getUniforms(const std::string &name) const;
};

class DirectionalLightModule : public LightModule {
  private:
    glm::vec3 direction;

  protected:
  public:
    DirectionalLightModule();
    DirectionalLightModule(glm::vec3 dir);
    DirectionalLightModule(glm::vec3 dir, glm::vec3 color);
    DirectionalLightModule(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    std::vector<std::shared_ptr<ShaderUniformVariableBase>> getUniforms(const std::string &name) const override;

    glm::vec3 getDirection() const;
};
} // namespace dawn_engine
