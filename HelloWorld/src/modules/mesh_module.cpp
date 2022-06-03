#include "mesh_module.h"

#include <utility>

namespace dawn_engine {

// Visual Shape Module
    const std::size_t MeshModule::type = BaseModule::getHashModuleName(quote(MeshModule));

    MeshModule::MeshModule() = default;

    MeshModule::MeshModule(std::vector<float> vertexData) : BaseModule(), vertexData(std::move(vertexData)), material(Material()) {
        this->initGLData();
    }

    MeshModule::MeshModule(std::vector<float> vertexData, Material material)
            : BaseModule(), vertexData(std::move(vertexData)), material(material) {
        this->initGLData();
    }

    MeshModule::~MeshModule() = default;

    void MeshModule::initGLData() {
        glGenVertexArrays(1, &this->glVAO);
        glBindVertexArray(this->glVAO);
        glGenBuffers(1, &this->glVBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->glVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertexData.data()) * this->vertexData.size(), this->vertexData.data(),
                     GL_STATIC_DRAW);
        const int posLen = 3;
        const int normalLen = 3;
        const int vertexSize = posLen + normalLen;
        const int data_offsets[] = {0, posLen};
        glVertexAttribPointer(0, posLen, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, normalLen, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float),
                              (void *) ((data_offsets[1]) * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    std::vector<float> MeshModule::getVertexData() const { return this->vertexData; }

    Material MeshModule::getMaterial() const { return this->material; }

    Material &MeshModule::getMaterialMeta() { return this->material; }

    uint32_t MeshModule::getVertexDataSize() const { return this->vertexData.size(); }

    void MeshModule::setAsRenderTarget() const { glBindVertexArray(this->glVAO); }
} // namespace dawn_engine
