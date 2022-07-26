//
// Created by zhiquan on 7/20/22.
//

#ifndef HELLOWORLD_GL_ALLOCATOR_H
#define HELLOWORLD_GL_ALLOCATOR_H

#include "core/dawn_vertex.h"
#include "core/camera.h"
#include "utils/resource_loader.h"

namespace dawn_engine {
    enum GLTextureFormat {
        RGB = GL_RGB,
        DepthComponent = GL_DEPTH_COMPONENT,
    };

    inline std::vector<unsigned int>
    AllocateGLVertexData(std::vector<DawnVertex> vertices, std::vector<unsigned int> indices) {
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
        glVertexAttribPointer(1, normalLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex),
                              (void *) offsetof(DawnVertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, texCoordLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex),
                              (void *) offsetof(DawnVertex, texCoords));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, tangentLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex),
                              (void *) offsetof(DawnVertex, tangent));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, biTangentLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex),
                              (void *) offsetof(DawnVertex, biTangent));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, boneIDsLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex),
                              (void *) offsetof(DawnVertex, mBoneIDs));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, boneWeightsLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex),
                              (void *) offsetof(DawnVertex, mWeights));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return {vao, vbo, ebo};
    }


    inline unsigned int AllocateGLTexture(GLTextureFormat format, glm::vec2 size, bool repeat = false) {
        unsigned int tex_id;
        glGenTextures(1, &tex_id);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if (repeat) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
        }
        return tex_id;
    }

    inline unsigned int GenGLDepthFramebuffer() {
        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return fbo;
    }

    inline void BindTexture2Framebuffer(unsigned int fbo, GLTexture texture) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, texture.attachment, texture.target, texture.id, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    inline std::vector<unsigned int>
    AllocateGLTextureDataFiltered(std::unordered_map<std::string, unsigned int> &loaded_texture_id_map, const
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
        unsigned int ubo;
        long buffer_size = 2 * sizeof(glm::mat4);
        unsigned int binding_idx = 0;
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, binding_idx, ubo, 0, buffer_size);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                        glm::value_ptr(camera.GetPerspectiveMatrix(win_width, win_height)));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                        glm::value_ptr(camera.GetViewMatrix()));
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
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(int),
                                &std::dynamic_pointer_cast<ShaderUniformVariable<int>>(uniform)->GetDataRef());
                buffer_offset += 4;
            } else if (uniform->GetTypeHash() == typeid(bool).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(int),
                                &std::dynamic_pointer_cast<ShaderUniformVariable<bool>>(uniform)->GetDataRef());
                buffer_offset += 4;
            } else if (uniform->GetTypeHash() == typeid(float).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(float),
                                &std::dynamic_pointer_cast<ShaderUniformVariable<float>>(uniform)->GetDataRef());
                buffer_offset += 4;
            } else if (uniform->GetTypeHash() == typeid(glm::vec3).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(glm::vec3),
                                glm::value_ptr(std::dynamic_pointer_cast<ShaderUniformVariable<glm::vec3>>(
                                        uniform)->GetDataRef()));
                buffer_offset += 16;
            } else if (uniform->GetTypeHash() == typeid(glm::mat4).hash_code()) {
                glBufferSubData(GL_UNIFORM_BUFFER, buffer_offset, sizeof(glm::mat4),
                                glm::value_ptr(std::dynamic_pointer_cast<ShaderUniformVariable<glm::mat4>>(
                                        uniform)->GetDataRef()));
                buffer_offset += 64;
            } else {
                throw std::runtime_error("unknown shader uniform variable type");
            }
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        return ubo;
    }
}
#endif //HELLOWORLD_GL_ALLOCATOR_H
