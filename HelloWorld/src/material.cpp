#include "material.h"

namespace dawn {
Material::Material()
    : ambient(glm::vec3(1.0f, 0.0f, 0.0f)), diffuse(glm::vec3(0.0f, 1.0f, 0.0f)), specular(glm::vec3(0.0f, 0.0f, 1.0f)),
      shininess(64.0f) {}
Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
    : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

glm::vec3 Material::getAmbient() const { return this->ambient; }
void Material::setAmbient(glm::vec3 value) { this->ambient = glm::normalize(value); }
glm::vec3 Material::getDiffuse() const { return this->diffuse; }
void Material::setDiffuse(glm::vec3 value) { this->diffuse = glm::normalize(value); }
glm::vec3 Material::getSpecular() const { return this->specular; }
void Material::setSpecular(glm::vec3 value) { this->specular = glm::normalize(value); }
float Material::getShininess() const { return this->shininess; }
void Material::setShinibess(float value) { this->shininess = shininess; }
} // namespace dawn