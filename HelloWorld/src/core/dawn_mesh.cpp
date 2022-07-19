#include "dawn_mesh.h"

#include <utility>

namespace dawn_engine {

    DawnMesh::DawnMesh() = default;

    DawnMesh::DawnMesh(const std::vector<DawnVertex> &vertices, const std::vector<unsigned int> &indices,const  std::shared_ptr<DawnMaterial>  &material_ptr) {
        this->vertices_ = vertices;
        this->indices_ = indices;
        this->material_ptr_ = material_ptr;
    }

//
//    void DawnMesh::InitGLData() {
//        glGenVertexArrays(1, &this->glVAO);
//        glGenBuffers(1, &this->glVBO);
//        glGenBuffers(1, &this->glEBO);
//        glBindVertexArray(this->glVAO);
//        glBindBuffer(GL_ARRAY_BUFFER, this->glVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(DawnVertex) * this->vertices_.size(), this->vertices_.data(), GL_STATIC_DRAW);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glEBO);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices_.size() * sizeof(unsigned int), this->indices_.data(), GL_STATIC_DRAW);
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
//
//    void DawnMesh::deleteGLData() {
//        glDeleteVertexArrays(1, &this->glVAO);
//        glDeleteBuffers(1, &this->glVBO);
//        glDeleteBuffers(1, &this->glEBO);
//    }

//    void DawnMesh::render(GLShaderProgram *shaderProgram) const {
//        shaderProgram->SetUniforms(this->material.getUniforms());
//        this->material.bindTextures();
//        glBindVertexArray(this->glVAO);
//        glDrawElements(GL_TRIANGLES, (int) this->indices_.size(), GL_UNSIGNED_INT, 0);
//        // unbind vao_ and texture
//        glBindVertexArray(0);
//        glActiveTexture(GL_TEXTURE0);
//
//    }

    unsigned int DawnMesh::GetVerticesNum() const {
        return this->vertices_.size();
    }

    unsigned int DawnMesh::GetIndicesNum() const {
        return this->indices_.size();
    }

    bool DawnMesh::EnableLightingMap() const {
        return this->material_ptr_->EnabledLightingMaps();
    }

    std::shared_ptr<DawnMaterial>  DawnMesh::GetMaterialPtr() const {
        return this->material_ptr_;
    }


    std::vector<DawnVertex> &DawnMesh::GetVerticesRef() {
        return this->vertices_;
    }

    std::vector<unsigned int> &DawnMesh::GetIndicesRef() {
        return this->indices_;
    }

    void DawnMesh::SetMaterialPtr(const std::shared_ptr<DawnMaterial> &material_ptr) {
        this->material_ptr_ = material_ptr;
    }


} // namespace dawn_engine
