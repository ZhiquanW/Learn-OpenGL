//
// Created by zhiquan on 6/6/22.
//

#include "dawn_model.h"

namespace dawn_engine {


    DawnModel::DawnModel(const std::string &path, bool gamma) {
        this->loadModel(path);
    }

    void DawnModel::Render(OpenGLShaderProgram *shaderProgram) {
        for (auto &mesh: this->meshes) {
            mesh.render(shaderProgram);
        }

    }

    void DawnModel::loadModel(const std::string &path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }
        this->directory = path.substr(0, path.find_last_of('/'));
        this->processNode(scene->mRootNode, scene);

    }

    void DawnModel::processNode(aiNode *node, const aiScene *scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            this->processNode(node->mChildren[i], scene);
        }
    }

    DawnMesh DawnModel::processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex{};
            // store position vector
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            // store normal vector
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
            if (mesh->mTextureCoords[0]) {
                // store texture coordinate
                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
                // store tangent
                if (mesh->mTangents != nullptr) {

                    vertex.tangent.x = mesh->mTangents[i].x;
                    vertex.tangent.y = mesh->mTangents[i].y;
                    vertex.tangent.z = mesh->mTangents[i].z;
                }
                // store bi tangent
                if (mesh->mBitangents != nullptr) {
                    vertex.biTangent.x = mesh->mBitangents[i].x;
                    vertex.biTangent.y = mesh->mBitangents[i].y;
                    vertex.biTangent.z = mesh->mBitangents[i].z;
                }
            } else {
                vertex.texCoords = glm::vec2(0.0f);
            }
            vertices.push_back(vertex);
        }
        // store indices
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }
        // store material
        std::vector<int> diffuseMaps = {};
        std::vector<int> specularMaps = {};
        std::vector<int> normalMaps = {};
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
            diffuseMaps = this->loadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE);
            specularMaps = this->loadMaterialTextures(meshMaterial, aiTextureType_SPECULAR);
            normalMaps = this->loadMaterialTextures(meshMaterial, aiTextureType_HEIGHT);
        }
        DawnMaterial material = {diffuseMaps, specularMaps, normalMaps};
        return {vertices, indices, material};
    }

    std::vector<int> DawnModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type) {
        std::vector<int> texturesID;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool isTexLoaded = false;
            for (auto &loaded_texture: textures) {
                if (std::strcmp(str.C_Str(), loaded_texture.path.data()) == 0) {
                    texturesID.push_back(loaded_texture.id);
                    isTexLoaded = true;
                    break;
                }
            }
            if (!isTexLoaded) {
                DawnTexture texture;
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.path = str.C_Str();
                texturesID.push_back(texture.id);
                textures.push_back(texture);
            }
        }
        return texturesID;
    }

    std::vector<DawnMesh> DawnModel::getMeshes() const {
        return this->meshes;

    }
} // dawn_engine