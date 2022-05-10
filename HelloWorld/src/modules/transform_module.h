#include "base_module.h"
namespace dawn_engine {
class TransformModule : public BaseModule {

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
    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;
    glm::mat4 getTranslationMat4() const;
    glm::mat4 getScaleMat4() const;
    glm::mat4 getRotationMat4() const;
    glm::mat4 getModelMat4() const;
};

} // namespace dawn_engine