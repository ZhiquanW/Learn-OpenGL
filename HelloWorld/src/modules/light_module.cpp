#include "light_module.h"

namespace dawn_engine {

const std::size_t LightModule::type = BaseModule::getHashModuleName(quote(LightModule));
const std::size_t DirectionalLightModule::type = BaseModule::getHashModuleName(quote(DirectionalLightModule));

LightModule::LightModule() : BaseModule(), ambient(glm::vec3(0.1f)), diffuse(glm::vec3(1.0f)), specular(glm::vec3(1.0f)) {}
LightModule::LightModule(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : BaseModule(), ambient(ambient), diffuse(diffuse), specular(specular) {}
LightModule::LightModule(glm::vec3 color) : BaseModule(), ambient(color), diffuse(color), specular(color) {}
LightModule::~LightModule(){};
glm::vec3 LightModule::getAmbient() const { return this->ambient; }
void LightModule::setAmbient(glm::vec3 value) { this->ambient = glm::normalize(value); }
glm::vec3 LightModule::getDiffuse() const { return this->diffuse; }
void LightModule::setDiffuse(glm::vec3 value) { this->diffuse = glm::normalize(value); }
glm::vec3 LightModule::getSpecular() const { return this->specular; }
void LightModule::setSpecular(glm::vec3 value) { this->specular = glm::normalize(value); }
// point light
std::vector<std::shared_ptr<ShaderUniformVariableBase>> LightModule::getUniforms(const std::string &name) const {
    std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms;
    light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".ambient", this->ambient));
    light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".diffuse", this->diffuse));
    light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".specular", this->specular));
    return light_uniforms;
}

// // direction light
DirectionalLightModule::DirectionalLightModule() : LightModule(), direction(glm::vec3(-1.0f)) {}
DirectionalLightModule::DirectionalLightModule(glm::vec3 dir) : LightModule(), direction(dir) {}
DirectionalLightModule::DirectionalLightModule(glm::vec3 direction, glm::vec3 color)
    : LightModule(color), direction(direction) {}
DirectionalLightModule::DirectionalLightModule(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : LightModule(ambient, diffuse, specular), direction(direction) {}
glm::vec3 DirectionalLightModule::getDirection() const { return this->direction; }
DirectionalLightModule::~DirectionalLightModule(){};
std::vector<std::shared_ptr<ShaderUniformVariableBase>> DirectionalLightModule::getUniforms(const uint32_t idx) const {
    std::string indexed_name = fmt::format("directional_lights[{}]", idx);
    std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = LightModule::getUniforms(indexed_name);
    light_uniforms.emplace_back(
        std::make_shared<ShaderUniformVariable<glm::vec3>>(indexed_name + ".direction", this->direction));
    return light_uniforms;
}
void DirectionalLightModule::setDirection(const glm::vec3 &dir) { this->direction = dir; }

} // namespace dawn_engine
