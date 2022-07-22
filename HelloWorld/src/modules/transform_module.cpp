#include "transform_module.h"

namespace dawn_engine {
//  Transform Module
    const std::size_t TransformModule::type = BaseModule::GetHashModuleName(quote(TransformModule));

    TransformModule::TransformModule()
            : BaseModule(), position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)) {}

    TransformModule::TransformModule(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
            : BaseModule(), position(position), rotation(rotation), scale(scale) {}

    TransformModule::~TransformModule() = default;

    glm::mat4 TransformModule::GetTranslationMat() const {
        glm::mat4 transMat = glm::mat4(1.0f);
        return glm::translate(transMat, this->position);
    }

    glm::mat4 TransformModule::GetRotationMat() const {
        glm::mat4 rotateMat = glm::mat4(1.0f);
        return glm::eulerAngleXZY(this->rotation.x, this->rotation.y, this->rotation.z);
    }

    glm::mat4 TransformModule::GetScaleMat() const {
        glm::mat4 scaleMat = glm::mat4(1.0f);
        return glm::scale(scaleMat, this->scale);
    }

    glm::mat4 TransformModule::GetModelMat() const {
        glm::mat4 modelMat = glm::mat4(1.0f);
        return this->GetTranslationMat() * this->GetRotationMat() * this->GetScaleMat();
    }

    glm::vec3 TransformModule::GetPosition() const {
        return this->position;
    }

    glm::vec3 &TransformModule::getPositionMeta() {
        return this->position;
    }

    [[maybe_unused]] glm::vec3 TransformModule::GetRotation() const {
        return this->rotation;
    }

    glm::vec3 &TransformModule::getRotationMeta() {
        return this->rotation;
    }

    [[maybe_unused]] glm::vec3 TransformModule::GetScale() const {
        return this->scale;
    }

    glm::vec3 &TransformModule::getScaleMeta() {
        return this->scale;
    }

    void TransformModule::SetPosition(const glm::vec3 &pos) {
        this->position = pos;
    }

    [[maybe_unused]] void TransformModule::setRotation(const glm::vec3 &rot) {
        this->rotation = rot;
    }

    void TransformModule::SetScale(const glm::vec3 &s) {
        this->scale = s;

    }

    glm::mat4 TransformModule::GetTransformMat() const {
        return GetTranslationMat() * GetRotationMat() * GetScaleMat();
    }

    glm::mat4 TransformModule::GetInvModelMat() const {
        return glm::inverse(this->GetModelMat());
    }


} // namespace dawn_engine