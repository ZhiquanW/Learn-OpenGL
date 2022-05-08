#include "light_module.h"

namespace dawn_engine {

LightModule::LightModule() : ambient(glm::vec3(1.0f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3(1.0f)) {}
LightModule::LightModule(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : ambient(ambient), diffuse(diffuse), specular(specular) {}
LightModule::LightModule(glm::vec3 color) : ambient(color), diffuse(color), specular(color) {}
LightModule::~LightModule() {}
glm::vec3 LightModule::getAmbient() const { return this->ambient; }
void LightModule::setAmbient(glm::vec3 value) { this->ambient = glm::normalize(value); }
glm::vec3 LightModule::getDiffuse() const { return this->diffuse; }
void LightModule::setDiffuse(glm::vec3 value) { this->diffuse = glm::normalize(value); }
glm::vec3 LightModule::getSpecular() const { return this->specular; }
void LightModule::setSpecular(glm::vec3 value) { this->specular = glm::normalize(value); }
// point light
std::vector<std::shared_ptr<ShaderUniformVariableBase>> LightModule::getUniforms(const std::string &name) const {
    std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms;
    uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".ambient", this->ambient));
    uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".diffuse", this->diffuse));
    uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".specular", this->specular));
    return uniforms;
}

// // direction light
DirectionalLightModule::DirectionalLightModule() : LightModule(), direction(glm::vec3(0.0f, -1.0f, 0.0f)) {}
DirectionalLightModule::DirectionalLightModule(glm::vec3 direction, glm::vec3 color)
    : LightModule(color), direction(direction) {}
DirectionalLightModule::DirectionalLightModule(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : LightModule(ambient, diffuse, specular), direction(direction) {}
glm::vec3 DirectionalLightModule::getDirection() const { return this->direction; }
} // namespace dawn_engine
