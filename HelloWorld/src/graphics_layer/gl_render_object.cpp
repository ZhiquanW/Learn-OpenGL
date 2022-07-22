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
              linked_shader_(DawnEngine::instance->GetShaderProgramMapMeta().at(shader_info.name)) {
    }

    GLRenderObject::GLRenderObject(GLRenderElement render_element,
                                   unsigned int vao,
                                   unsigned int vbo,
                                   unsigned int ebo,
                                   unsigned int indices_num,
                                   const ShaderInfo &shader_info)
            : render_element_(render_element),
              vao_(vao),
              vbo_(vbo),
              ebo_(ebo),
              indices_num_(indices_num),
              linked_shader_(DawnEngine::instance->GetShaderProgramMapMeta().at(shader_info.name)) {

    }

    GLRenderObject::GLRenderObject(unsigned int vao,
                                   unsigned int vbo,
                                   unsigned int ebo,
                                   unsigned int indices_num,
                                   const std::vector<unsigned int>& diffuse_tex_ids,
                                   const std::vector<unsigned int>& specular_tex_ids,
                                   const std::vector<unsigned int>& normal_tex_ids,
                                   const ShaderInfo &shader_info)
            : vao_(vao),
              vbo_(vbo),
              ebo_(ebo),
              indices_num_(indices_num),
              linked_shader_(DawnEngine::instance->GetShaderProgramMapMeta().at(shader_info.name)) {
        for(auto texture_id : diffuse_tex_ids){
            this->AppendGLTexture(texture_id);
        }
        for(auto texture_id : specular_tex_ids){
            this->AppendGLTexture(texture_id);
        }
        for(auto texture_id : normal_tex_ids){
            this->AppendGLTexture(texture_id);
        }
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
              linked_shader_(DawnEngine::instance->GetShaderProgramMapMeta().at(shader_info.name)) {
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
        for(auto pair: this->texture2d_id_map_){
            glActiveTexture(GL_TEXTURE0+pair.second);
            glBindTexture(GL_TEXTURE_2D, pair.first);
        }
        unsigned int common_texture_num = this->texture_counter_;
        if(this->cube_map_tex_id_ !=-1){
            glActiveTexture(GL_TEXTURE0 + common_texture_num++);
            glBindTexture(GL_TEXTURE_CUBE_MAP, this->cube_map_tex_id_);
        }
//        if(this->depth_map_tex_id != -1){
//            glActiveTexture(GL_TEXTURE0 + common_texture_num++);
//            glBindTexture(GL_TEXTURE_2D, this->depth_map_tex_id);
//        }
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
        this->linked_shader_->Activate();
        this->linked_shader_->SetUniforms(this->uniform_map_);
        this->BindGLData();
        switch (this->render_element_) {
            case GLRenderElement::TRIANGLE:
                glDrawElements(GL_TRIANGLES, (int) this->indices_num_, GL_UNSIGNED_INT, 0);
                break;
            case GLRenderElement::LINE:
                glDrawArrays(GL_LINES, 0, 2);

                break;

        }


    }

    void GLRenderObject::RefreshUniforms(std::vector<std::shared_ptr<ShaderUniformVariableBase>> uniforms) {
//        this->uniforms_ = std::move(uniforms);
        for (const auto &uniform : uniforms) {
            if (this->uniform_map_.count(uniform->GetName()) == 0) {
                this->uniform_map_.insert({uniform->GetName(), uniform});
            } else {
                this->uniform_map_.at(uniform->GetName()) = uniform;
            }
        }

    }

    void GLRenderObject::RefreshShaderProgram(GLShaderProgram *shader_program) {
        this->linked_shader_ = shader_program;
    }

    unsigned int GLRenderObject::ApplyTextureIdx()  {
        return this->texture_counter_++;
    }

    unsigned int GLRenderObject::AppendGLTexture(unsigned int texture_id) {
        if(this->texture2d_id_map_.count(texture_id) > 0){
            return this->texture2d_id_map_.at(texture_id);
        }
         this->texture2d_id_map_.insert({texture_id, this->texture_counter_,});
         return this->texture_counter_++;
    }

//    void GLRenderObject::SetDepthTexture(unsigned int texture_id) {
//        this->depth_map_tex_id = texture_id;
//    }


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