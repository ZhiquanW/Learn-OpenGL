#pragma once
#include "common_includes.h"
#include "shader_uniform_variable.h"
namespace dawn_engine {
class Light {
  protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

  public:
    Light();
    Light(glm::vec3 color);
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    virtual ~Light() = 0;

    glm::vec3 getAmbient() const;
    void setAmbient(glm::vec3 value);
    glm::vec3 getDiffuse() const;
    void setDiffuse(glm::vec3 value);
    glm::vec3 getSpecular() const;
    void setSpecular(glm::vec3 value);
    float getShininess() const;
    void setShinibess(float value);
};

class PointLight : public Light {
  protected:
    glm::vec3 position;

  public:
    PointLight();
    PointLight(glm::vec3 position, glm::vec3 color);
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    glm::vec3 getPosition() const;
    void setPosition(glm::vec3 position);
    std::vector<std::shared_ptr<ShaderUniformVariableBase>> getUniforms(const std::string &name) const;
};

class DirectionalLight : public Light {
  protected:
    glm::vec3 direction;

  public:
    DirectionalLight();
    DirectionalLight(glm::vec3 direction, glm::vec3 color);
    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    glm::vec3 getDirection() const;
    void setDirection(glm::vec3 direction);
};
} // namespace dawn_engine
