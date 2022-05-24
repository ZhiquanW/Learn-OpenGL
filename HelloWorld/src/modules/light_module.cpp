#include "light_module.h"

namespace dawn_engine {

const std::size_t LightModule::type = BaseModule::getHashModuleName(quote(LightModule));
const std::size_t DirectionalLightModule::type = BaseModule::getHashModuleName(quote(DirectionalLightModule));
const std::size_t PointLightModule::type = BaseModule::getHashModuleName(quote(PointLightModule));

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
// base light
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

// point light
PointLightModule::PointLightModule() : LightModule() {}
PointLightModule::PointLightModule(glm::vec3 color, float constant, float linear, float quadratic)
    : LightModule(color), constant(constant), linear(linear), quadratic(quadratic) {}
PointLightModule::PointLightModule(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear,
                                   float quadratic)
    : LightModule(ambient, diffuse, specular), constant(constant), linear(linear), quadratic(quadratic) {}

float PointLightModule::getConstant() const { return this->constant; }
float PointLightModule::getLinear() const { return this->linear; }
float PointLightModule::getQuadratic() const { return this->quadratic; }

std::vector<std::shared_ptr<ShaderUniformVariableBase>> PointLightModule::getUniforms(const uint32_t idx) const {
    std::string indexed_name = fmt::format("point_lights[{}]", idx);
    std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = LightModule::getUniforms(indexed_name);
    light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".constant", this->constant));
    light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".linear", this->linear));
    light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".quadratic", this->quadratic));
    light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(
        indexed_name + ".position", this->getAttachedGameObject()->getModule<TransformModule>()->getPosition()));
    return light_uniforms;
}
} // namespace dawn_engine
