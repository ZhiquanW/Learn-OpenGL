#include "module.h"

namespace dawn {
Module::Module() {}
const std::size_t Module::getHashModuleName(const char *moduleType) { return std::hash<std::string>()(moduleType); }

std::shared_ptr<GameObject> Module::getAttachedGameObjet() const { return this->gameObjectPtr; }

// Visual Shape Module
const std::size_t VisualShapeModule::type = Module::getHashModuleName(quote(VisualShapeModule));
VisualShapeModule::VisualShapeModule() {}
VisualShapeModule::VisualShapeModule(std::vector<float> vertexData) : vertexData(vertexData), material(Material()) {
    this->initGLData();
}

VisualShapeModule::VisualShapeModule(std::vector<float> vertexData, Material material)
    : vertexData(vertexData), material(material) {
    this->initGLData();
}

VisualShapeModule::~VisualShapeModule() {}

void VisualShapeModule::initGLData() {
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
    glVertexAttribPointer(0, posLen, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, normalLen, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float),
                          (void *)((data_offsets[1]) * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::vector<float> VisualShapeModule::getVertexData() const { return this->vertexData; }
Material VisualShapeModule::getMaterial() const { return this->material; }
uint32_t VisualShapeModule::getVertexDataSize() const { return this->vertexData.size(); }
void VisualShapeModule::setRenderTarget() const { glBindVertexArray(this->glVAO); }

//  Transform Module
const std::size_t TransformModule::type = Module::getHashModuleName(quote(TransformModule));

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
} // namespace dawn