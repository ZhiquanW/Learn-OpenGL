//
// Created by zhiquan on 6/6/22.
//

#ifndef HELLOWORLD_DAWN_MODEL_H
#define HELLOWORLD_DAWN_MODEL_H

#include "dawn_mesh.h"


namespace dawn_engine {

//    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);
    struct DawnTexture {
        int id;
        std::string path;
    };

    class DawnModel {
    private:
        std::vector<DawnTexture> textures;
        std::vector<DawnMesh> meshes;

        std::string directory;

        void loadModel(const std::string &path);

        void processNode(aiNode *node, const aiScene *scene);

        DawnMesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<int> loadMaterialTextures(aiMaterial *mat, aiTextureType type);

    protected:
    public:
        DawnModel() = default;

        explicit DawnModel(const std::string &path, bool gamma = false);

        void Render(OpenGLShaderProgram *shaderProgram);

        [[nodiscard]] std::vector<DawnMesh> getMeshes() const;

    };

} // dawn_engine

#endif //HELLOWORLD_DAWN_MODEL_H
