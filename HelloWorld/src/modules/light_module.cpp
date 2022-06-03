#include "light_module.h"

#include <cmath>

namespace dawn_engine {

    const std::size_t LightModule::type = BaseModule::getHashModuleName(quote(LightModule));
    const std::size_t DirectionalLightModule::type = BaseModule::getHashModuleName(
            quote(DirectionalLightModule));
    const std::size_t PointLightModule::type = BaseModule::getHashModuleName(
            quote(PointLightModule));
    const std::size_t SpotLightModule::type = BaseModule::getHashModuleName(
            quote(SpotLightModule));


    LightModule::LightModule()
            : BaseModule(), ambient(glm::vec3(0.1f)), diffuse(glm::vec3(1.0f)),
              specular(glm::vec3(1.0f)) {}

    LightModule::LightModule(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : BaseModule(), ambient(ambient), diffuse(diffuse), specular(specular) {}

    LightModule::LightModule(glm::vec3 color)
            : BaseModule(), ambient(color), diffuse(color), specular(color) {}

    LightModule::~LightModule() = default;


// base light
    std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    LightModule::getUniforms(const std::string &name) const {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms;
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".ambient",
                                                                   this->ambient));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".diffuse",
                                                                   this->diffuse));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".specular",
                                                                   this->specular));
        return light_uniforms;
    }

    glm::vec3 LightModule::getAmbient() const {
        return this->ambient;
    }

    glm::vec3 &LightModule::getAmbientMeta() {
        return this->ambient;
    }

    glm::vec3 LightModule::getDiffuse() const {
        return this->diffuse;
    }

    glm::vec3 &LightModule::getDiffuseMeta() {
        return this->diffuse;
    }

    glm::vec3 LightModule::getSpecular() const {
        return this->specular;
    }

    glm::vec3 &LightModule::getSpecularMeta() {
        return this->specular;
    }


// direction light
    DirectionalLightModule::DirectionalLightModule()
            : LightModule(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)), direction(glm::vec3(-1.0f)) {}

    DirectionalLightModule::DirectionalLightModule(glm::vec3 dir)
            : LightModule(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)), direction(dir) {}

    DirectionalLightModule::DirectionalLightModule(glm::vec3 direction, glm::vec3 color)
            : LightModule(color), direction(direction) {}

    DirectionalLightModule::DirectionalLightModule(glm::vec3 direction, glm::vec3 ambient,
                                                   glm::vec3 diffuse,
                                                   glm::vec3 specular)
            : LightModule(ambient, diffuse, specular), direction(direction) {}


    DirectionalLightModule::~DirectionalLightModule() = default;;

    glm::vec3 DirectionalLightModule::getDirection() const { return glm::normalize(this->direction); }

    glm::vec3 &DirectionalLightModule::getDirectionMeta() {
        this->direction = glm::normalize(this->direction);
        return this->direction;
    }

    void DirectionalLightModule::setDirection(const glm::vec3 &dir) { this->direction = dir; }

    std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    DirectionalLightModule::getUniforms(const uint32_t idx) const {
        std::string indexed_name = fmt::format("directional_lights[{}]", idx);
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = LightModule::getUniforms(
                indexed_name);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(indexed_name + ".direction",
                                                                   this->direction));
        return light_uniforms;
    }


// point light
    PointLightModule::PointLightModule() : LightModule(), constant(1.0), linear(0.1), quadratic(0.08) {}

    PointLightModule::PointLightModule(glm::vec3 color, float constant, float linear,
                                       float quadratic)
            : LightModule(color), constant(constant), linear(linear), quadratic(quadratic) {}

    PointLightModule::PointLightModule(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                                       float constant,
                                       float linear,
                                       float quadratic)
            : LightModule(ambient, diffuse, specular), constant(constant), linear(linear),
              quadratic(quadratic) {}

    std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    PointLightModule::getUniforms(const uint32_t idx) const {
        std::string indexed_name = fmt::format("point_lights[{}]", idx);
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = LightModule::getUniforms(
                indexed_name);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".constant",
                                                               this->constant));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".linear",
                                                               this->linear));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".quadratic",
                                                               this->quadratic));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(
                indexed_name + ".position",
                this->getAttachedGameObject()->getModule<TransformModule>()->getPosition()));
        return light_uniforms;
    }

    float PointLightModule::getConstant() const {
        return this->constant;
    }

    float &PointLightModule::getConstantMeta() {
        return this->constant;
    }

    void PointLightModule::setConstant(const float &c) {
        this->constant = c;

    }

    float PointLightModule::getLinear() const {
        return this->linear;
    }

    float &PointLightModule::getLinearMeta() {
        return this->linear;
    }

    void PointLightModule::setLinear(const float &l) {
        this->linear = l;
    }

    float PointLightModule::getQuadratic() const {
        return this->quadratic;
    }

    float &PointLightModule::getQuadraticMeta() {
        return this->quadratic;
    }

    void PointLightModule::setQuadratic(const float &q) {
        this->quadratic = q;
    }


    SpotLightModule::SpotLightModule() : PointLightModule(), direction(glm::vec3(-1.0f)), innerRange(15), outerRange(30) {
    }

    SpotLightModule::SpotLightModule(glm::vec3 dir, float innerRange, float outerRange) : PointLightModule(), direction(dir), innerRange(innerRange), outerRange(outerRange) {
    }

    std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    SpotLightModule::getUniforms(const uint32_t idx) const {
        std::string indexed_name = fmt::format("spot_lights[{}]", idx);
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = LightModule::getUniforms(
                indexed_name);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(indexed_name + ".direction",
                                                                   this->getDirection()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(indexed_name + ".position",
                                                                   this->getAttachedGameObject()->getModule<TransformModule>()->getPosition()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".linear",
                                                               this->getLinear()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".constant",
                                                               this->getConstant()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".quadratic",
                                                               this->getQuadratic()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".inner_range",
                                                               this->getInnerRange()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".outer_range",
                                                               this->getOuterRange()));
        return light_uniforms;
    }

    glm::vec3 SpotLightModule::getDirection() const { return glm::normalize(this->direction); }

    glm::vec3 &SpotLightModule::getDirectionMeta() {
        this->direction = glm::normalize(this->direction);
        return this->direction;
    }

    void SpotLightModule::setDirection(const glm::vec3 &dir) { this->direction = dir; }

    float SpotLightModule::getInnerRange() const {
        return this->innerRange;
    }

    float &SpotLightModule::getInnerRangeMeta() {
        return this->innerRange;
    }

    void SpotLightModule::setInnerRange(const float &range) {
        this->innerRange = std::min(this->outerRange, range);
    }

    float SpotLightModule::getOuterRange() const {
        return this->outerRange;
    }

    float &SpotLightModule::getOuterRangeMeta() {
        return this->outerRange;
    }

    void SpotLightModule::setOuterRange(const float &range) {
        this->outerRange = std::max(this->innerRange, range);
    }


} // namespace dawn_engine
