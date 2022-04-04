#pragma once
#include "common_includes.h"
#include "material.h"
namespace dawn {

enum PrimitiveType { CubePrimitiveType, SpherePrimitiveType };
class GameObject;
class Module {

  protected:
    static const std::size_t type;
    std::shared_ptr<GameObject> gameObjectPtr;

  public:
    friend class GameObject;
    Module();
    virtual ~Module(){};
    virtual std::shared_ptr<GameObject> getAttachedGameObjet() const;
    static const std::size_t getHashModuleName(const char *moduleType);
};

class VisualShapeModule : public Module {
  private:
    unsigned int glVAO, glVBO, glEBO;
    Material material;
    void initGLData();

  protected:
    std::vector<float> vertexData;

  public:
    static const std::size_t type;
    VisualShapeModule();
    VisualShapeModule(std::vector<float> vertexData);
    VisualShapeModule(std::vector<float> vertexData, Material material);
    ~VisualShapeModule();

    std::vector<float> getVertexData() const;
    Material getMaterial() const;
    std::vector<uint32_t> getIndices() const;
    uint32_t getIndicesSize() const;
    uint32_t getVertexDataSize() const;
    void setRenderTarget() const;
};

class TransformModule : public Module {

  protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

  public:
    static const std::size_t type;

    TransformModule();
    ~TransformModule();
    TransformModule(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 rotation);
    glm::mat4 getTranslationMat4() const;
    glm::mat4 getScaleMat4() const;
    glm::mat4 getRotationMat4() const;
    glm::mat4 getModelMat4() const;
};
} // namespace dawn