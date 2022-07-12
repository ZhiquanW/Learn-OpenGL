//
// Created by zhiquan on 6/6/22.
//

#ifndef HELLOWORLD_DAWN_MODEL_H
#define HELLOWORLD_DAWN_MODEL_H

#include "common_includes.h"
#include "dawn_mesh.h"
#include "dawn_texture.h"
#include "utils/gl_transformer.h"

namespace dawn_engine {

//    unsigned int LoadTexture2GL(const char *path, const std::string &directory, bool gamma);


    class DawnModel {
    private:
        std::vector<DawnMesh> meshes_ = {};
        std::vector<std::shared_ptr<DawnTexture >> textures_ = {};

        std::string directory = {};

        void LoadModel(const std::string &path);

        void processNode(aiNode *node, const aiScene *scene);

        DawnMesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<std::shared_ptr<DawnTexture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type);

    protected:
    public:
        DawnModel() = default;

        explicit DawnModel(const std::string &path, bool gamma = false);

        DawnModel(const std::vector<DawnMesh> &meshes);

//        void Render(GLShaderProgram *shaderProgram);

        [[nodiscard]] std::vector<DawnMesh> GetMeshes() const;

        std::vector<DawnMesh> &GetMeshesRef();

        unsigned int GetMeshNum() const;

        void SetAllTransparent(float alpha);

        void EnableAllTransparent();

    };

} // dawn_engine

#endif //HELLOWORLD_DAWN_MODEL_H
