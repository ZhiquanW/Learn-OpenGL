#pragma once
#include "common_includes.h"
namespace dawn {
class Material {
  protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

  public:
    Material();
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
    glm::vec3 getAmbient() const;
    void setAmbient(glm::vec3 value);
    glm::vec3 getDiffuse() const;
    void setDiffuse(glm::vec3 value);
    glm::vec3 getSpecular() const;
    void setSpecular(glm::vec3 value);
    float getShininess() const;
    void setShinibess(float value);
};
} // namespace dawn