//
// Created by zhiquan on 6/23/22.
//

#include "gl_render_object.h"
#include "core/dawn_engine.h"

#include <utility>

namespace dawn_engine {

    GLRenderObject::GLRenderObject(unsigned int vao,
                                   unsigned int vbo,
                                   unsigned int ebo,
                                   unsigned int indices_num,
                                   const ShaderInfo &shader_info)
            : vao_(vao),
              vbo_(vbo),
              ebo_(ebo),
              indices_num_(indices_num),
              linked_shader_(DawnEngine::engineInstance->getShaderProgramMapMeta().at(shader_info.name)) {
    }

    GLRenderObject::GLRenderObject(unsigned int vao,
                                   unsigned int vbo,
                                   unsigned int ebo,
                                   unsigned int indices_num,
                                   std::vector<unsigned int> diffuse_tex_ids,
                                   std::vector<unsigned int> specular_tex_ids,
                                   std::vector<unsigned int> normal_tex_ids,
                                   const ShaderInfo &shader_info)
            : vao_(vao),
              vbo_(vbo),
              ebo_(ebo),
              indices_num_(indices_num),
              diffuse_tex_ids_(std::move(diffuse_tex_ids)),
              specular_tex_ids_(std::move(specular_tex_ids)),
              normal_tex_ids_(std::move(normal_tex_ids)),
              linked_shader_(DawnEngine::engineInstance->getShaderProgramMapMeta().at(shader_info.name)) {
    }

    GLRenderObject::GLRenderObject(unsigned int vao,
                                   unsigned int vbo,
                                   unsigned int ebo,
                                   unsigned int indices_num,
                                   unsigned int cube_map_tex_id,
                                   const ShaderInfo &shader_info)
            : vao_(vao),
              vbo_(vbo),
              ebo_(ebo),
              indices_num_(indices_num),
              cube_map_tex_id_(cube_map_tex_id),
              linked_shader_(DawnEngine::engineInstance->getShaderProgramMapMeta().at(shader_info.name)) {
    }

    GLRenderObject::~GLRenderObject() {
        this->ReleaseGLData();
    }


    void GLRenderObject::ReleaseGLData() {
        glDeleteVertexArrays(1, &this->vao_);
        glDeleteBuffers(1, &this->vbo_);
        glDeleteBuffers(1, &this->ebo_);

    }

    void GLRenderObject::BindGLData() const {
        glBindVertexArray(this->vao_);
        // bind textures
        int texture_unit_idx = 0;
        for (auto tex_id: this->diffuse_tex_ids_) {
            glActiveTexture(GL_TEXTURE0 + texture_unit_idx++);
            glBindTexture(GL_TEXTURE_2D, tex_id);
        }
        for (auto tex_id: this->specular_tex_ids_) {
            glActiveTexture(GL_TEXTURE0 + texture_unit_idx++);
            glBindTexture(GL_TEXTURE_2D, tex_id);
        }
        for (auto tex_id: this->normal_tex_ids_) {
            glActiveTexture(GL_TEXTURE0 + texture_unit_idx++);
            glBindTexture(GL_TEXTURE_2D, tex_id);
        }
        glActiveTexture(GL_TEXTURE0 + texture_unit_idx++);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cube_map_tex_id_);
    }

    void GLRenderObject::UnbindGLData() const {
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

    void GLRenderObject::render() const {
//        std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms = ExtractUniforms("material", linked_mesh_->GetMaterialRef());
//        std::vector<std::shared_ptr<ShaderUniformVariableBase>> transform_uniforms = ExtractUniforms("model_mat", this->linked_game_object_->GetModule<TransformModule>());
//        std::vector<std::shared_ptr<ShaderUniformVariableBase>> tex_uniforms = ExtractTexUniforms("material", this->diffuse_tex_ids_, this->specular_tex_ids_,
//                                                                                                  this->normal_tex_ids_, this->cube_map_tex_id_);
//        uniforms.insert(uniforms.end(), transform_uniforms.begin(), transform_uniforms.end());
//        uniforms.insert(uniforms.end(), tex_uniforms.begin(), tex_uniforms.end());
        this->linked_shader_->activate();
        this->linked_shader_->setUniforms(this->uniforms_);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 1);
        this->BindGLData();
        glDrawElements(GL_TRIANGLES, (int) this->indices_num_, GL_UNSIGNED_INT, 0);
        this->UnbindGLData();


    }

    void GLRenderObject::RefreshUniforms(std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms) {
        this->uniforms_ = uniforms;

    }

//    void GLRenderObject::AllocateGLData(std::vector<DawnVertex> vertices, std::vector<unsigned int> indices) {
//        glGenVertexArrays(1, &this->vao_);
//        glGenBuffers(1, &this->vbo_);
//        glGenBuffers(1, &this->ebo_);
//        glBindVertexArray(this->vao_);
//        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(DawnVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
//        const int posLen = 3;
//        const int normalLen = 3;
//        const int texCoordLen = 2;
//        const int tangentLen = 3;
//        const int biTangentLen = 3;
//        const int boneIDsLen = MAX_BONE_INFLUENCE;
//        const int boneWeightsLen = MAX_BONE_INFLUENCE;
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, posLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) nullptr);
//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, normalLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, normal));
//        glEnableVertexAttribArray(2);
//        glVertexAttribPointer(2, texCoordLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, texCoords));
//        glEnableVertexAttribArray(3);
//        glVertexAttribPointer(3, tangentLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, tangent));
//        glEnableVertexAttribArray(4);
//        glVertexAttribPointer(4, biTangentLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, biTangent));
//        glEnableVertexAttribArray(5);
//        glVertexAttribPointer(5, boneIDsLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, mBoneIDs));
//        glEnableVertexAttribArray(6);
//        glVertexAttribPointer(6, boneWeightsLen, GL_FLOAT, GL_FALSE, sizeof(DawnVertex), (void *) offsetof(DawnVertex, mWeights));
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//    }



//    DawnMesh *GLRenderObject::GetLinkedMesh() const {
//        return this->linked_mesh_;
//
//    }

}