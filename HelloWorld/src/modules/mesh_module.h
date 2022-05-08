#include "base_module.h"

namespace dawn_engine {

class MeshModule : public BaseModule {
  private:
    unsigned int glVAO, glVBO, glEBO;
    Material material;
    void initGLData();

  protected:
    std::vector<float> vertexData;

  public:
    static const std::size_t type;
    MeshModule();
    MeshModule(std::vector<float> vertexData);
    MeshModule(std::vector<float> vertexData, Material material);
    ~MeshModule();

    std::vector<float> getVertexData() const;
    Material getMaterial() const;
    std::vector<uint32_t> getIndices() const;
    uint32_t getIndicesSize() const;
    uint32_t getVertexDataSize() const;
    void setAsRenderTarget() const;
};

} // namespace dawn_engine