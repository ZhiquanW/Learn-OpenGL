#include "transform_module.h"

namespace dawn_engine {
//  Transform Module
    const std::size_t TransformModule::type = BaseModule::getHashModuleName(quote(TransformModule));

    TransformModule::TransformModule()
            : BaseModule(), position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)) {}

    TransformModule::TransformModule(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
            : BaseModule(), position(position), rotation(rotation), scale(scale) {}

    TransformModule::~TransformModule() = default;

    glm::mat4 TransformModule::getTranslationMat4() const {
        glm::mat4 transMat = glm::mat4(1.0f);
        return glm::translate(transMat, this->position);
    }

    glm::mat4 TransformModule::getRotationMat4() const {
        glm::mat4 rotateMat = glm::mat4(1.0f);
        return glm::eulerAngleXZY(this->rotation.x, this->rotation.y, this->rotation.z);
    }

    glm::mat4 TransformModule::getScaleMat4() const {
        glm::mat4 scaleMat = glm::mat4(1.0f);
        return glm::scale(scaleMat, this->scale);
    }

    glm::mat4 TransformModule::GetModelMat4() const {
        glm::mat4 modelMat = glm::mat4(1.0f);
        return this->getTranslationMat4() * this->getRotationMat4() * this->getScaleMat4();
    }

    glm::vec3 TransformModule::GetPosition() const {
        return this->position;
    }

    glm::vec3 &TransformModule::getPositionMeta() {
        return this->position;
    }

    [[maybe_unused]] glm::vec3 TransformModule::getRotation() const {
        return this->rotation;
    }

    glm::vec3 &TransformModule::getRotationMeta() {
        return this->rotation;
    }

    [[maybe_unused]] glm::vec3 TransformModule::getScale() const {
        return this->scale;
    }

    glm::vec3 &TransformModule::getScaleMeta() {
        return this->scale;
    }

    void TransformModule::setPosition(const glm::vec3 &pos) {
        this->position = pos;
    }

    [[maybe_unused]] void TransformModule::setRotation(const glm::vec3 &rot) {
        this->rotation = rot;
    }

    void TransformModule::setScale(const glm::vec3 &s) {
        this->scale = s;

    }


} // namespace dawn_engine