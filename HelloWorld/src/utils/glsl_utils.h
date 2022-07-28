//
// Created by zhiquan on 6/15/22.
//

#ifndef GLSL_UTILS_H
#define GLSL_UTILS_H

#include "core/camera.h"
#include "core/game_object.h"
#include "modules/light_module.h"

namespace dawn_engine {

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    ExtractUniforms(const std::string &name, Camera camera, float win_width, float win_height) {
        return {
                std::make_shared<ShaderUniformVariable<glm::mat4 >>(fmt::format("{}.projection", name),
                                                                    camera.GetPerspectiveMatrix(win_width, win_height)),
                std::make_shared<ShaderUniformVariable<glm::mat4 >>(fmt::format("{}.view", name),
                                                                    camera.GetViewMatrix()),
                std::make_shared<ShaderUniformVariable<glm::vec3 >>(fmt::format("{}.pos", name), camera.GetPosition()),
                std::make_shared<ShaderUniformVariable<float >>(fmt::format("{}.z_near", name), camera.GetZNear()),
                std::make_shared<ShaderUniformVariable<float >>(fmt::format("{}.z_far", name), camera.GetZFar()),

        };
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    ExtractUniforms(const std::string &name, std::shared_ptr<DawnMaterial> material_ptr) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = {
                std::make_shared<ShaderUniformVariable<bool>>(fmt::format("{}.enable_lighting_maps", name),
                                                              material_ptr->EnabledLightingMaps()),
                std::make_shared<ShaderUniformVariable<bool>>(fmt::format("{}.opaque", name),
                                                              material_ptr->GetOpaque()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(fmt::format("{}.ambient", name),
                                                                   material_ptr->GetAmbient()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(fmt::format("{}.diffuse", name),
                                                                   material_ptr->GetDiffuse()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(fmt::format("{}.specular", name),
                                                                   material_ptr->GetSpecular()),
                std::make_shared<ShaderUniformVariable<float>>(fmt::format("{}.shininess", name),
                                                               material_ptr->GetShininess()),
                std::make_shared<ShaderUniformVariable<float>>(fmt::format("{}.transparency", name),
                                                               fmin(1.0f - float(material_ptr->GetOpaque()),
                                                                    material_ptr->GetTransparency()))
        };
        return uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractTexUniforms(const std::string &name,
                                                                                      std::shared_ptr<DawnMaterial> material_ptr) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = ExtractUniforms(name, material_ptr);
        unsigned int tex_unit_idx = GLShaderProgram::GetGlobalTextureSpace();
        unsigned int diffuse_idx = 0;
        for (int i = 0; i < material_ptr->GetDiffuseTextures().size(); ++i) {
            std::string uniform_name = fmt::format("{}.diffuse_texture_{}", name, diffuse_idx++);
            uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>(uniform_name, tex_unit_idx++));
        }
        unsigned int specular_idx = 0;
        for (int i = 0; i < material_ptr->GetSpecularTextures().size(); ++i) {
            std::string uniformName = fmt::format("{}.specular_texture_{}", name, specular_idx++);
            uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>(uniformName, tex_unit_idx++));
        }
        unsigned int normal_idx = 0;

        for (int i = 0; i < material_ptr->GetNormalTextures().size(); ++i) {
            std::string uniformName = fmt::format("{}.normal_texture_{}", name, normal_idx++);
            uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>(uniformName, tex_unit_idx++));
        }
        uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<int>>(fmt::format("{}.cubemap", name), tex_unit_idx++));
//        uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>(fmt::format("material.cubemap", name), tex_unit_idx++));
        return uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    ExtractUniforms(const std::string &name, LightModule *lightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = {
                std::make_shared<ShaderUniformVariable<int>>(name + ".activation", lightModule->GetActivation()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".ambient_", lightModule->GetAmbient()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".diffuse", lightModule->GetDiffuse()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".specular", lightModule->GetSpecular())};
//                std::make_shared<ShaderUniformVariable<int>>(name + ".shadow_map",
//                                                             lightModule->GetShadowMapTexture().unit)};
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    ExtractUniforms(const std::string &name, DirectionalLightModule *lightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = ExtractUniforms(name,
                                                                                                 (LightModule *) lightModule);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".direction", lightModule->GetDirection()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::mat4>>(name + ".light_space_transform_mat",
                                                                   lightModule->GetLightSpaceTransformMat()));
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    ExtractUniforms(const std::string &name, SpotLightModule *spotLightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = ExtractUniforms(name,
                                                                                                 (LightModule *) spotLightModule);
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".direction",
                                                                                       spotLightModule->getDirection()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".position",
                                                                   spotLightModule->GetAttachedGameObject()->GetModule<TransformModule>()->GetPosition()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(name + ".constant", spotLightModule->getConstant()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(name + ".linear", spotLightModule->getLinear()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(name + ".quadratic", spotLightModule->getQuadratic()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".inner_range",
                                                                                   spotLightModule->getInnerRange()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".outer_range",
                                                                                   spotLightModule->getOuterRange()));
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    ExtractUniforms(const std::string &name, PointLightModule *pointLightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = ExtractUniforms(name,
                                                                                                 (LightModule *) pointLightModule);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".position",
                                                                   pointLightModule->GetAttachedGameObject()->GetModule<TransformModule>()->GetPosition()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(name + ".linear", pointLightModule->getLinear()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(name + ".constant", pointLightModule->getConstant()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(name + ".quadratic", pointLightModule->getQuadratic()));
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    ExtractUniforms(const std::string &name, TransformModule *transformModule) {
        return {std::make_shared<ShaderUniformVariable<glm::mat4>>(name, transformModule->GetModelMat())};
    }


}


#endif