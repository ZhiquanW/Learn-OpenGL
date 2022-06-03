#include "base_module.h"

namespace dawn_engine {

    class MeshModule : public BaseModule {
    private:
        Material material;
        unsigned int glVAO{}, glVBO{}, glEBO{};

        void initGLData();

    protected:
        std::vector<float> vertexData;

    public:
        static const std::size_t type;

        MeshModule();

        explicit MeshModule(std::vector<float> vertexData);

        MeshModule(std::vector<float> vertexData, Material material);

        ~MeshModule();

        [[nodiscard]] std::vector<float> getVertexData() const;

        [[nodiscard]] Material getMaterial() const;

        [[nodiscard]] Material &getMaterialMeta();

        [[nodiscard]] std::vector<uint32_t> getIndices() const;

        [[nodiscard]] uint32_t getIndicesSize() const;

        uint32_t getVertexDataSize() const;

        void setAsRenderTarget() const;
    };

} // namespace dawn_engine