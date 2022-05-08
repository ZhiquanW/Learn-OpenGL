#include "hello_world_app.h"
#include <memory>
namespace hello {

HelloWorldApp::HelloWorldApp(uint width, uint height) : DawnEngine(width, height, "hello world") {
    // renderEngine = std::make_shared<dawn_engine::DawnEngine>(width, height, "hello world");
}
void HelloWorldApp::addDefaultLight() {
    // std::shared_ptr<dawn_engine::PointLight> pLight(
    //     new dawn_engine::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
    // this->addLight(pLight);
}

void HelloWorldApp::addDefaultCube() {
    // std::shared_ptr<dawn_engine::PointLight> pLight(
    //     new dawn_engine::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
    // this->addLight(pLight);
    // this->addGameObject(dawn_engine::GameObject::createPrimitive(dawn_engine::CubePrimitiveType));
    // this->addGameObject(->gameObjects.emplace_back(GameObject::createPrimitive(CubePrimitiveType));
}

void HelloWorldApp::awake() {}
void HelloWorldApp::start() {
    // std::shared_ptr<dawn_engine::PointLight> pLight(
    //     new dawn_engine::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
    // this->addLight(pLight);

    // this->addGameObject(dawn_engine::GameObject::createPrimitive(dawn_engine::CubePrimitiveType));
    auto tmpObj = dawn_engine::GameObject::createPrimitive(dawn_engine::CubePrimitiveType);
    this->addGameObject(tmpObj);
}
void HelloWorldApp::update() {}
} // namespace hello