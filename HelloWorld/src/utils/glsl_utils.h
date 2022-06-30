//
// Created by zhiquan on 6/15/22.
//

#ifndef GLSL_UTILS_H
#define GLSL_UTILS_H

#include "core/camera.h"
#include "core/game_object.h"
#include "modules/light_module.h"

namespace dawn_engine {

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, Camera camera, float winWidth, float winHeight) {
        return {
                std::make_shared<ShaderUniformVariable<glm::mat4 >>(fmt::format("{}.view", name), camera.GetViewMatrix()),
                std::make_shared<ShaderUniformVariable<glm::mat4 >>(fmt::format("{}.projection", name), camera.getPerspectiveMatrix(winWidth, winHeight)),
                std::make_shared<ShaderUniformVariable<glm::vec3 >>(fmt::format("{}.pos", name), camera.GetPosition()),
                std::make_shared<ShaderUniformVariable<float >>(fmt::format("{}.z_near", name), camera.getZNear()),
                std::make_shared<ShaderUniformVariable<float >>(fmt::format("{}.z_far", name), camera.getZFar()),

        };
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, const DawnMaterial &material) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = {
                std::make_shared<ShaderUniformVariable<bool>>(fmt::format("{}.enable_lighting_maps", name), material.EnabledLightingMaps()),
                std::make_shared<ShaderUniformVariable<bool>>(fmt::format("{}.opaque", name), material.GetOpaque()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(fmt::format("{}.ambient", name), material.GetAmbient()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(fmt::format("{}.diffuse", name), material.GetDiffuse()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(fmt::format("{}.specular", name), material.GetSpecular()),
                std::make_shared<ShaderUniformVariable<float>>(fmt::format("{}.shininess", name), material.GetShininess()),
                std::make_shared<ShaderUniformVariable<float>>(fmt::format("{}.transparency", name),
                                                               fmin(1.0f - float(material.GetOpaque()), material.GetTransparency()))
        };
        return uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractTexUniforms(const std::string &name,
                                                                                      const DawnMaterial &material) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = {};
        unsigned int tex_unit_idx = 0;
        unsigned int diffuse_idx = 0;
        for (int i = 0; i < material.GetDiffuseTextures().size(); ++i) {
            std::string uniform_name = fmt::format("material.diffuse_texture_{}", diffuse_idx++);
            uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>(uniform_name, tex_unit_idx++));
        }
        unsigned int specular_idx = 0;
        for (int i = 0; i < material.GetSpecularTextures().size(); ++i) {
            std::string uniformName = fmt::format("material.specular_texture_{}", specular_idx++);
            uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>(uniformName, tex_unit_idx++));
        }
        unsigned int normal_idx = 0;

        for (int i = 0; i < material.GetNormalTextures().size(); ++i) {
            std::string uniformName = fmt::format("material.normal_texture_{}", normal_idx++);
            uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>(uniformName, tex_unit_idx++));
        }
        uniforms.emplace_back(std::make_shared<ShaderUniformVariable<int>>("material.cubemap", tex_unit_idx++));
        return uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, LightModule *lightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = {
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".ambient", lightModule->getAmbient()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".diffuse", lightModule->getDiffuse()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".specular", lightModule->getSpecular())};

        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, DirectionalLightModule *lightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = ExtractUniforms(name, (LightModule *) lightModule);
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".direction", lightModule->getDirection()));
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, SpotLightModule *spotLightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = ExtractUniforms(name, (LightModule *) spotLightModule);
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".direction", spotLightModule->getDirection()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".position", spotLightModule->GetAttachedGameObject()->GetModule<TransformModule>()->GetPosition()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".linear", spotLightModule->getLinear()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".constant", spotLightModule->getConstant()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".quadratic", spotLightModule->getQuadratic()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".inner_range", spotLightModule->getInnerRange()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".outer_range", spotLightModule->getOuterRange()));
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, PointLightModule *pointLightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = ExtractUniforms(name, (LightModule *) pointLightModule);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".position", pointLightModule->GetAttachedGameObject()->GetModule<TransformModule>()->GetPosition()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".linear", pointLightModule->getLinear()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".constant", pointLightModule->getConstant()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".quadratic", pointLightModule->getQuadratic()));
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, TransformModule *transformModule) {
        return {std::make_shared<ShaderUniformVariable<glm::mat4>>(name, transformModule->GetModelMat4())};
    }


    inline std::vector<unsigned int> AllocateGLVertexData(std::vector<DawnVertex> vertices, std::vector<unsigned int> indices) {
        unsigned int vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(DawnVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        const int posLen = 3;
        const int normalLen = 3;
        const int texCoordLen = 2;
        const int tangentLen = 3;
        const int biTangentLen = 3;
        const int boneIDsLen = MAX_BONE_INFLUENCE;
        const int boneWeightsLen = MAX_BONE_INFLUENCE;
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, posLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, normalLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, texCoordLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, texCoords));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, tangentLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, tangent));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, biTangentLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, biTangent));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, boneIDsLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, mBoneIDs));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, boneWeightsLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, mWeights));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return {vao, vbo, ebo};
    }

//    inline std::vector<unsigned int> AllocateTextureData(const std::vector<std::shared_ptr<DawnTexture>> &textures) {
//        std::vector<unsigned int> texture_ids = {};
//        for (const auto &texture: textures) {
//            texture_ids.push_back(LoadTexture2GL(texture, true));
//        }
//        return texture_ids;
//    }

    inline std::vector<unsigned int> AllocateGLTextureDataFiltered(std::unordered_map<std::string, unsigned int> &loaded_texture_id_map, const
    std::vector<std::shared_ptr<DawnTexture>> &
    textures) {
        std::vector<unsigned int> gl_texture_ids = {};
        for (const auto &tex: textures) {
            if (loaded_texture_id_map.find(tex->GetName()) != loaded_texture_id_map.end()) {
                gl_texture_ids.push_back(loaded_texture_id_map.at(tex->GetName()));
            } else {
                unsigned gl_tex_id = LoadTexture2GL(tex, true);
                gl_texture_ids.push_back(gl_tex_id);
                loaded_texture_id_map.insert({tex->GetName(), gl_tex_id});
            }
        }
        return gl_texture_ids;
    }


}


#endif