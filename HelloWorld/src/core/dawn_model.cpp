//
// Created by zhiquan on 6/6/22.
//

#include "dawn_model.h"

namespace dawn_engine {


    DawnModel::DawnModel(const std::string &path, bool gamma) {
        this->loadModel(path);
    }

    DawnModel::DawnModel(const std::vector<DawnMesh> &meshes)
            : meshes_(meshes) {
    }

//    void DawnModel::Render(GLShaderProgram *shaderProgram) {
//        for (auto &mesh: this->meshes_) {
//            mesh.render(shaderProgram);
//        }
//
//    }

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
            this->meshes_.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            this->processNode(node->mChildren[i], scene);
        }
    }

    DawnMesh DawnModel::processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<DawnVertex> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            DawnVertex vertex{};
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
        // store indices_
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }
        // store material
        std::vector<std::shared_ptr<DawnTexture>> diffuse_textures = {};
        std::vector<std::shared_ptr<DawnTexture>> specular_textures = {};
        std::vector<std::shared_ptr<DawnTexture>> normal_textures = {};
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
            diffuse_textures = this->loadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE);
            specular_textures = this->loadMaterialTextures(meshMaterial, aiTextureType_SPECULAR);
            normal_textures = this->loadMaterialTextures(meshMaterial, aiTextureType_HEIGHT);
        }
        DawnMaterial material = {diffuse_textures, specular_textures, normal_textures};
        return {vertices, indices, material};
    }

    std::vector<std::shared_ptr<DawnTexture>> DawnModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type) {
        // search through all textures linked to the material (*mat),
        // if loaded, add to target textures list else load texture to model and add it to target texture list
        std::vector<std::shared_ptr<DawnTexture>> target_textures = {};
        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
            aiString name;
            mat->GetTexture(type, i, &name);
            bool isTexLoaded = false;
            for (const auto &loadedTexture: this->textures_) {
                std::string texName = std::filesystem::path(name.C_Str()).stem();
                if (std::strcmp(texName.c_str(), loadedTexture->GetName().data()) == 0) {
                    target_textures.emplace_back(loadedTexture);
                    isTexLoaded = true;
                    break;
                }
            }
            if (!isTexLoaded) {
                auto new_texture = std::make_shared<DawnTexture>(fmt::format("{}/{}", directory, name.C_Str()), true);
                this->textures_.push_back(new_texture);
                target_textures.push_back(new_texture);
            }
        }
        return target_textures;
    }

    std::vector<DawnMesh> DawnModel::GetMeshes() const {
        return this->meshes_;

    }

    std::vector<DawnMesh> &DawnModel::GetMeshesRef() {
        return this->meshes_;
    }

    unsigned int DawnModel::GetMeshSize() const {
        return this->meshes_.size();
    }


} // dawn_engine