#include "transform_module.h"
namespace dawn_engine {
//  Transform Module
const std::size_t TransformModule::type = BaseModule::getHashModuleName(quote(TransformModule));

TransformModule::TransformModule() : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)) {}
TransformModule::TransformModule(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : position(position), rotation(rotation), scale(scale) {}

TransformModule::~TransformModule() {}

void TransformModule::setPosition(glm::vec3 position) { this->position = position; }
void TransformModule::setRotation(glm::vec3 rotation) { this->rotation = rotation; }
void TransformModule::setScale(glm::vec3 scale) { this->scale = scale; }
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
glm::mat4 TransformModule::getModelMat4() const {
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, this->position);
    return modelMat;
}
} // namespace dawn_engine