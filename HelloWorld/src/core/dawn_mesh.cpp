#include "dawn_mesh.h"

#include <utility>

namespace dawn_engine {

    int TextureFromFile(const char *path, const std::string &directory, bool gamma) {
        std::string fileName = fmt::format("{}/{}", directory, path);
        unsigned int textureID;
        glGenTextures(1, &textureID);
        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1) {
                format = GL_RED;
            } else if (nrComponents == 3) {
                format = GL_RGB;
            } else if (nrComponents == 4) {
                format = GL_RGBA;
            }
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            stbi_image_free(data);
        } else {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }
        return textureID;
    }
// Visual Shape Module

    DawnMesh::DawnMesh() = default;

    DawnMesh::DawnMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const DawnMaterial &material) {
        this->vertices = vertices;
        this->indices = indices;
        this->material = material;
    }


    DawnMesh::~DawnMesh() {

    }

    void DawnMesh::initGLData() {
        glGenVertexArrays(1, &this->glVAO);
        glGenBuffers(1, &this->glVBO);
        glGenBuffers(1, &this->glEBO);
        glBindVertexArray(this->glVAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->glVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);
        const int posLen = 3;
        const int normalLen = 3;
        const int texCoordLen = 2;
        const int tangentLen = 3;
        const int biTangentLen = 3;
        const int boneIDsLen = MAX_BONE_INFLUENCE;
        const int boneWeightsLen = MAX_BONE_INFLUENCE;
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, posLen, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, normalLen, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, texCoordLen, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, tangentLen, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tangent));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, biTangentLen, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, biTangent));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, boneIDsLen, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, mBoneIDs));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, boneWeightsLen, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, mWeights));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void DawnMesh::deleteGLData() {
        glDeleteVertexArrays(1, &this->glVAO);
        glDeleteBuffers(1, &this->glVBO);
        glDeleteBuffers(1, &this->glEBO);
    }

    void DawnMesh::render(OpenGLShaderProgram *shaderProgram) const {
        shaderProgram->setUniforms(this->material.getUniforms());
        this->material.bindTextures();
        glBindVertexArray(this->glVAO);
        glDrawElements(GL_TRIANGLES, (int) this->indices.size(), GL_UNSIGNED_INT, 0);
        // unbind vao and texture
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

    }

    unsigned int DawnMesh::verticesNum() const {
        return this->vertices.size();
    }

    bool DawnMesh::enableLightingMap() const {
        return this->material.enabledLightingMaps();
    }

    DawnMaterial DawnMesh::getMaterial() const {
        return this->material;
    }

    DawnMaterial &DawnMesh::getMaterialMeta() {
        return this->material;
    }


} // namespace dawn_engine
