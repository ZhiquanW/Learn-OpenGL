#ifndef DAWNENGINE_RESOURCE_LOADER
#define DAWNENGINE_RESOURCE_LOADER

#include "common_includes.h"
#include "core/dawn_texture.h"
#include "core/dawn_mesh.h"
#include "core/dawn_model.h"
#include "core/dawn_vertex.h"

inline unsigned int loadTexture2GL(const char *path, const std::string &directory, bool gamma) {
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
        glTexImage2D(GL_TEXTURE_2D, 0, (int) format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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

inline unsigned int LoadTexture2GL(const std::shared_ptr<dawn_engine::DawnTexture>& texture, bool gamma) {
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    if (texture->GetData()) {
        GLenum format;
        if (texture->getChannel() == 1) {
            format = GL_RED;
        } else if (texture->getChannel() == 3) {
            format = GL_RGB;
        } else if (texture->getChannel() == 4) {
            format = GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, (int) format, texture->getWidth(), texture->getHeight(), 0, format, GL_UNSIGNED_BYTE, texture->GetData());
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        throw std::runtime_error(fmt::format("Dawn Texture:{} - failed to load", texture->GetName()));
    }
    return texture_id;
}

inline unsigned char *loadTexture2Data(const char *name, const std::string &directory, bool gamma) {
    std::string filePath = fmt::format("{}/{}", directory, name);
    int width, height, channel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &channel, 0);
    if (data) {
        GLenum format;
        if (channel == 1) {
            format = GL_RED;
        } else if (channel == 3) {
            format = GL_RGB;
        } else if (channel == 4) {
            format = GL_RGBA;
        }
    } else {
        std::cout << "Texture failed to load at path: " << filePath << std::endl;
    }
    return data;

}

inline unsigned int LoadCubeMap2GL(const std::vector<std::shared_ptr<dawn_engine::DawnTexture>> &textures) {
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    for (unsigned int i = 0; i < textures.size(); ++i) {
        unsigned char *data = textures[i]->GetData();
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textures[i]->getWidth(), textures[i]->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//            stbi_image_free(data);
        } else {
//            stbi_image_free(data);
            throw std::runtime_error(fmt::format("Dawn Texture:{} - failed to load", textures[i]->GetName()));
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texture_id;
}

//namespace model_loading {
//
//
//    std::vector<std::shared_ptr<dawn_engine::DawnTexture>> LoadMaterialTextures(aiMaterial *mat, aiTextureType type) {
//        // search through all textures linked to the material (*mat),
//        // if loaded, add to target textures list else load texture to model and add it to target texture list
//        std::vector<std::shared_ptr<dawn_engine::DawnTexture>> target_textures = {};
//        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
//            aiString name;
//            mat->GetTexture(type, i, &name);
//            bool isTexLoaded = false;
//            for (const auto &loadedTexture: this->textures_) {
//                std::string texName = std::filesystem::path(name.C_Str()).stem();
//                if (std::strcmp(texName.c_str(), loadedTexture->GetName().data()) == 0) {
//                    target_textures.emplace_back(loadedTexture);
//                    isTexLoaded = true;
//                    break;
//                }
//            }
//            if (!isTexLoaded) {
//                auto new_texture = std::make_shared<dawn_engine::DawnTexture>(fmt::format("{}/{}", directory, name.C_Str()));
//                this->textures_.emplace_back(new_texture);
//                target_textures.emplace_back(new_texture);
//            }
//        }
//        return target_textures;
//    }
//
//    dawn_engine::DawnMesh ProcessMesh(aiMesh *mesh, const aiScene *scene) {
//        std::vector<dawn_engine::DawnVertex> vertices;
//        std::vector<unsigned int> indices;
//
//        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
//            dawn_engine::DawnVertex vertex{};
//            // store position vector
//            vertex.position.x = mesh->mVertices[i].x;
//            vertex.position.y = mesh->mVertices[i].y;
//            vertex.position.z = mesh->mVertices[i].z;
//            // store normal vector
//            vertex.normal.x = mesh->mNormals[i].x;
//            vertex.normal.y = mesh->mNormals[i].y;
//            vertex.normal.z = mesh->mNormals[i].z;
//            if (mesh->mTextureCoords[0]) {
//                // store texture coordinate
//                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
//                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
//                // store tangent
//                if (mesh->mTangents != nullptr) {
//
//                    vertex.tangent.x = mesh->mTangents[i].x;
//                    vertex.tangent.y = mesh->mTangents[i].y;
//                    vertex.tangent.z = mesh->mTangents[i].z;
//                }
//                // store bi tangent
//                if (mesh->mBitangents != nullptr) {
//                    vertex.biTangent.x = mesh->mBitangents[i].x;
//                    vertex.biTangent.y = mesh->mBitangents[i].y;
//                    vertex.biTangent.z = mesh->mBitangents[i].z;
//                }
//            } else {
//                vertex.texCoords = glm::vec2(0.0f);
//            }
//            vertices.push_back(vertex);
//        }
//        // store indices_
//        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
//            aiFace face = mesh->mFaces[i];
//            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
//                indices.push_back(face.mIndices[j]);
//            }
//        }
//        // store material
//        std::vector<std::shared_ptr<dawn_engine::DawnTexture>> diffuse_textures = {};
//        std::vector<std::shared_ptr<dawn_engine::DawnTexture>> specular_textures = {};
//        std::vector<std::shared_ptr<dawn_engine::DawnTexture>> normal_textures = {};
//        if (mesh->mMaterialIndex >= 0) {
//            aiMaterial *meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
//            diffuse_textures = LoadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE);
//            specular_textures = LoadMaterialTextures(meshMaterial, aiTextureType_SPECULAR);
//            normal_textures = LoadMaterialTextures(meshMaterial, aiTextureType_HEIGHT);
//        }
//        dawn_engine::DawnMaterial material = {diffuse_textures, specular_textures, normal_textures};
//        return {vertices, indices, material};
//    }
//
//
//    inline void ProcessNode(aiNode *node, const aiScene *scene, std::vector<dawn_engine::DawnMesh> &meshes) {
//        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
//            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
//            meshes.push_back(ProcessMesh(mesh, scene));
//        }
//        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
//            ProcessNode(node->mChildren[i], scene, meshes);
//        }
//    }
//
//    inline dawn_engine::DawnModel LoadModel(const std::string &path) {
//        Assimp::Importer importer;
//        std::vector<dawn_engine::DawnMesh> meshes;
//
//        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
//            return {};
//        }
//        const std::string &directory = path.substr(0, path.find_last_of('/'));
//        ProcessNode(scene->mRootNode, scene, meshes);
//    }
//
//
//}
#endif
