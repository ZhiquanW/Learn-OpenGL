//
// Created by zhiquan on 6/15/22.
//

#ifndef GLSL_UTILS_H
#define GLSL_UTILS_H

#include "core/camera.h"
#include "core/game_object.h"
#include "modules/light_module.h"

namespace dawn_engine {

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, Camera camera, float win_width, float win_height) {
        return {
                std::make_shared<ShaderUniformVariable<glm::mat4 >>(fmt::format("{}.projection", name), camera.GetPerspectiveMatrix(win_width, win_height)),
                std::make_shared<ShaderUniformVariable<glm::mat4 >>(fmt::format("{}.view", name), camera.GetViewMatrix()),
                std::make_shared<ShaderUniformVariable<glm::vec3 >>(fmt::format("{}.pos", name), camera.GetPosition()),
                std::make_shared<ShaderUniformVariable<float >>(fmt::format("{}.z_near", name), camera.GetZNear()),
                std::make_shared<ShaderUniformVariable<float >>(fmt::format("{}.z_far", name), camera.GetZFar()),

        };
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name,  std::shared_ptr<DawnMaterial> material_ptr) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = {
                std::make_shared<ShaderUniformVariable<bool>>(fmt::format("{}.enable_lighting_maps", name), material_ptr->EnabledLightingMaps()),
                std::make_shared<ShaderUniformVariable<bool>>(fmt::format("{}.opaque", name), material_ptr->GetOpaque()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(fmt::format("{}.ambient", name), material_ptr->GetAmbient()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(fmt::format("{}.diffuse", name), material_ptr->GetDiffuse()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(fmt::format("{}.specular", name), material_ptr->GetSpecular()),
                std::make_shared<ShaderUniformVariable<float>>(fmt::format("{}.shininess", name), material_ptr->GetShininess()),
                std::make_shared<ShaderUniformVariable<float>>(fmt::format("{}.transparency", name),
                                                               fmin(1.0f - float(material_ptr->GetOpaque()), material_ptr->GetTransparency()))
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
                std::make_shared<ShaderUniformVariable<int>>(name + ".activation", lightModule->GetActivation()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".ambient_", lightModule->GetAmbient()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".diffuse", lightModule->GetDiffuse()),
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".specular", lightModule->GetSpecular())};

        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, DirectionalLightModule *lightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = ExtractUniforms(name, (LightModule *) lightModule);
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".direction", lightModule->GetDirection()));
        return light_uniforms;
    }

    inline std::vector<std::shared_ptr<ShaderUniformVariableBase>> ExtractUniforms(const std::string &name, SpotLightModule *spotLightModule) {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = ExtractUniforms(name, (LightModule *) spotLightModule);
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".direction", spotLightModule->getDirection()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".position", spotLightModule->GetAttachedGameObject()->GetModule<TransformModule>()->GetPosition()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".constant", spotLightModule->getConstant()));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<float>>(name + ".linear", spotLightModule->getLinear()));
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


    inline unsigned int AllocateGLTexture(glm::vec2 size){
        unsigned int tex_id;
        glGenTextures(1, &tex_id);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,size.x,size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        return  tex_id;
    }

    inline unsigned int AllocateGLDepthMap(glm::vec2 size) {
        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        unsigned int tex_id = AllocateGLTexture(size);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_id, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return fbo;
    }

    inline std::vector<unsigned int> AllocateGLTextureDataFiltered(std::unordered_map<std::string, unsigned int> &loaded_texture_id_map, const
    std::vector<std::shared_ptr<DawnTexture>> &textures) {
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

    inline unsigned int AllocateUniformBlock(Camera camera, float win_width, float win_height) {
        // todo: 1. input: camera 2. input: uniforms 3. divide update with dynamics and statics
        unsigned int ubo;
        long buffer_size = 2 * sizeof(glm::mat4);
        unsigned int binding_idx = 0;
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, binding_idx, ubo, 0, buffer_size);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.GetPerspectiveMatrix(win_width, win_height)));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetViewMatrix()));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        return ubo;
    }

    inline unsigned int AllocateUniformBlock(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        unsigned int ubo;
        auto uniforms_sizes = dawn_engine::ShaderUniformVariableBase::GetUniformsDataSizes(uniforms);
        long buffer_size = 0;
        for (auto uniform_size: uniforms_sizes) {
            buffer_size += uniform_size;
        }

        unsigned int binding_idx = 0;
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, binding_idx, ubo, 0, buffer_size);
        long buffer_offset = 0;
        for (const auto &uniform: uniforms) {
            if (uniform->GetTypeHash() == typeid(int).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(int), &std::dynamic_pointer_cast<ShaderUniformVariable<int>>(uniform)->GetDataRef());
                buffer_offset += 4;
            } else if (uniform->GetTypeHash() == typeid(bool).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(int), &std::dynamic_pointer_cast<ShaderUniformVariable<bool>>(uniform)->GetDataRef());
                buffer_offset += 4;
            } else if (uniform->GetTypeHash() == typeid(float).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(float), &std::dynamic_pointer_cast<ShaderUniformVariable<float>>(uniform)->GetDataRef());
                buffer_offset += 4;
            } else if (uniform->GetTypeHash() == typeid(glm::vec3).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(glm::vec3),
                                glm::value_ptr(std::dynamic_pointer_cast<ShaderUniformVariable<glm::vec3>>(uniform)->GetDataRef()));
                buffer_offset += 16;
            } else if (uniform->GetTypeHash() == typeid(glm::mat4).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(glm::mat4),
                                glm::value_ptr(std::dynamic_pointer_cast<ShaderUniformVariable<glm::mat4>>(uniform)->GetDataRef()));
                buffer_offset += 64;
            } else {
                throw std::runtime_error("unknown shader uniform variable type");
            }
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        return ubo;
    }
}


#endif