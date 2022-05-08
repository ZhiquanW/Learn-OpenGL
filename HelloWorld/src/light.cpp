#include "light.h"

namespace dawn_engine {
Light::Light() : ambient(glm::vec3(1.0f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3(1.0f)) {}
Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : ambient(ambient), diffuse(diffuse), specular(specular) {}
Light::Light(glm::vec3 color) : ambient(color), diffuse(color), specular(color) {}
Light::~Light() {}
glm::vec3 Light::getAmbient() const { return this->ambient; }
void Light::setAmbient(glm::vec3 value) { this->ambient = glm::normalize(value); }
glm::vec3 Light::getDiffuse() const { return this->diffuse; }
void Light::setDiffuse(glm::vec3 value) { this->diffuse = glm::normalize(value); }
glm::vec3 Light::getSpecular() const { return this->specular; }
void Light::setSpecular(glm::vec3 value) { this->specular = glm::normalize(value); }
// point light
PointLight::PointLight() : Light(), position(glm::vec3(0.0f)) {}
PointLight::PointLight(glm::vec3 position, glm::vec3 color) : Light(color), position(position) {}
PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(ambient, diffuse, specular), position(position) {}
glm::vec3 PointLight::getPosition() const { return this->position; }
void PointLight::setPosition(glm::vec3 position) { this->position = position; }
std::vector<std::shared_ptr<ShaderUniformVariableBase>> PointLight::getUniforms(const std::string &name) const {
    std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms;
    uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".position", this->position));
    uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".ambient", this->ambient));
    uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".diffuse", this->diffuse));
    uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".specular", this->specular));
    return uniforms;
}

// direction light
DirectionalLight::DirectionalLight() : Light(), direction(glm::vec3(0.0f, -1.0f, 0.0f)) {}
DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color)
    : Light(color), direction(glm::vec3(0.0f, -1.0f, 0.0f)) {}
DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : Light(ambient, diffuse, specular), direction(direction) {}
glm::vec3 DirectionalLight::getDirection() const { return this->direction; }
void DirectionalLight::setDirection(glm::vec3 direction) { this->direction = direction; }

} // namespace dawn_engine