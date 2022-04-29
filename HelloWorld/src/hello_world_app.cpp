#include "hello_world_app.h"
#include <memory>
namespace hello {

HelloWorldApp::HelloWorldApp(uint width, uint height) : DawnEngine(width, height, "hello world") {
    // renderEngine = std::make_shared<dawn::DawnEngine>(width, height, "hello world");
}
void HelloWorldApp::addDefaultLight() {
    // std::shared_ptr<dawn::PointLight> pLight(
    //     new dawn::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
    // this->addLight(pLight);
}

void HelloWorldApp::addDefaultCube() {
    // std::shared_ptr<dawn::PointLight> pLight(
    //     new dawn::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
    // this->addLight(pLight);
    // this->addGameObject(dawn::GameObject::createPrimitive(dawn::CubePrimitiveType));
    // this->addGameObject(->gameObjects.emplace_back(GameObject::createPrimitive(CubePrimitiveType));
}

void HelloWorldApp::awake() {}
void HelloWorldApp::start() {
    std::shared_ptr<dawn::PointLight> pLight(
        new dawn::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
    this->addLight(pLight);

    this->addGameObject(dawn::GameObject::createPrimitive(dawn::CubePrimitiveType));
}
void HelloWorldApp::update() {}
} // namespace hello