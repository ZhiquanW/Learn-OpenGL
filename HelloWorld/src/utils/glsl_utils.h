//
// Created by zhiquan on 6/15/22.
//

#ifndef GLSL_UTILS_H
#define GLSL_UTILS_H

#include "core/camera.h"
#include "modules/light_module.h"

namespace dawn_engine {

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> extractUniforms(const std::string &name, Camera camera, float winWidth, float winHeight) {
        return {
                std::make_shared<ShaderUniformVariable<glm::mat4 >>(fmt::format("{}.view", name), camera.getViewMatrix()),
                std::make_shared<ShaderUniformVariable<glm::mat4 >>(fmt::format("{}.projection", name), camera.getPerspectiveMatrix(winWidth, winHeight)),
                std::make_shared<ShaderUniformVariable<glm::vec3 >>(fmt::format("{}.pos", name), camera.getPos()),
                std::make_shared<ShaderUniformVariable<float >>(fmt::format("{}.z_near", name), camera.getZNear()),
                std::make_shared<ShaderUniformVariable<float >>(fmt::format("{}.z_far", name), camera.getZFar()),

        };
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> extractUniforms(const std::string &name, LightModule *lightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = {
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".ambient", lightModule->getAmbient()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".diffuse", lightModule->getDiffuse()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".specular", lightModule->getSpecular())};

        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> extractUniforms(const std::string &name, DirectionalLightModule *lightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = extractUniforms(name, (LightModule *) lightModule);
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".direction", lightModule->getDirection()));
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> extractUniforms(const std::string &name, SpotLightModule *spotLightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = extractUniforms(name, (LightModule *) spotLightModule);
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".direction", spotLightModule->getDirection()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".position", spotLightModule->getAttachedGameObject()->getModule<TransformModule>()->getPosition()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".linear", spotLightModule->getLinear()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".constant", spotLightModule->getConstant()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".quadratic", spotLightModule->getQuadratic()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".inner_range", spotLightModule->getInnerRange()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".outer_range", spotLightModule->getOuterRange()));
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> extractUniforms(const std::string &name, PointLightModule *pointLightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = extractUniforms(name, (LightModule *) pointLightModule);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".position", pointLightModule->getAttachedGameObject()->getModule<TransformModule>()->getPosition()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".linear", pointLightModule->getLinear()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".constant", pointLightModule->getConstant()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".quadratic", pointLightModule->getQuadratic()));
        return light_uniforms;
    }

}

#endif