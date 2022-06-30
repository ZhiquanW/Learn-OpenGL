#include "hello_world_app.h"
#include <memory>
#include "core/dawn_ui_system.h"
#include "helloworld_ui_system.h"

namespace helloworld {

    HelloWorldApp::HelloWorldApp(uint width, uint height) : DawnEngine(width, height,
                                                                       "hello world") {
        auto *uiSystemPtr = new HelloWorldUISystem(430);
        this->mountUISystem(static_cast<dawn_engine::DawnUISystem *>(uiSystemPtr));
        // renderEngine = std::make_shared<dawn_engine::DawnEngine>(width, height, "hello world");
    }

//    void HelloWorldApp::addDefaultLight() {
//        // std::shared_ptr<dawn_engine::PointLight> pLight(
//        //     new dawn_engine::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
//        // this->addLight(pLight);
//    }
//
//    void HelloWorldApp::addDefaultCube() {
//        // std::shared_ptr<dawn_engine::PointLight> pLight(
//        //     new dawn_engine::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
//        // this->addLight(pLight);
//        // this->addGameObject(dawn_engine::GameObject::createPrimitive(dawn_engine::CubePrimitiveType));
//        // this->addGameObject(->gameObjects.emplace_back(GameObject::createPrimitive(CubePrimitiveType));
//    }

    void HelloWorldApp::awake() {


    }

    void HelloWorldApp::start() {
        // std::shared_ptr<dawn_engine::PointLight> pLight(
        //     new dawn_engine::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
        // this->addLight(pLight);
        // this->addGameObject(dawn_engine::GameObject::createPrimitive(dawn_engine::CubePrimitiveType));
        // load resource

        auto skyboxObj = dawn_engine::GameObject::createSkybox({
                                                                       "../assets/skybox/right.jpg", "../assets/skybox/left.jpg",
                                                                       "../assets/skybox/top.jpg", "../assets/skybox/bottom.jpg",
                                                                       "../assets/skybox/front.jpg", "../assets/skybox/back.jpg",
                                                               });
        this->addSkybox(skyboxObj);
        dawn_engine::DawnModel backpackModel = dawn_engine::DawnModel("../assets/backpack/backpack.obj");
        auto backpack_obj = new dawn_engine::GameObject();
        backpack_obj->addModule<dawn_engine::RendererModule>(backpackModel);
        this->addGameObject(backpack_obj);
//         add game object

        auto opaque_cube = dawn_engine::GameObject::createPrimitive(dawn_engine::CubePrimitiveType);
        opaque_cube->GetModule<dawn_engine::TransformModule>()->setPosition(
                glm::vec3(2.0, 0.0, 0.0));
        this->addGameObject(opaque_cube);
        auto transparent_cube = dawn_engine::GameObject::createPrimitive(dawn_engine::CubePrimitiveType);
        transparent_cube->GetModule<dawn_engine::TransformModule>()->setPosition(
                glm::vec3(-2.0, 0.0, 0.0));
        this->addGameObject(transparent_cube);
        auto tmpLight = dawn_engine::GameObject::createLight(
                dawn_engine::LightType::DirectionalLightType);
        tmpLight->GetModule<dawn_engine::DirectionalLightModule>()->setDirection(
                glm::vec3(1.0, 0, 0));
        this->addGameObject(tmpLight);
        auto tmpPLight = dawn_engine::GameObject::createLight(
                dawn_engine::LightType::PointLightType);
        tmpPLight->GetModule<dawn_engine::TransformModule>()->setPosition(glm::vec3(0, 0, 1));
        this->addGameObject(tmpPLight);
        auto tmpSLight = dawn_engine::GameObject::createLight(
                dawn_engine::LightType::SpotLightType);
        tmpSLight->GetModule<dawn_engine::TransformModule>()->setPosition(
                glm::vec3(0, 0, 1));
        tmpSLight->GetModule<dawn_engine::SpotLightModule>()->setDirection(
                glm::vec3(0.0, 0, -1));
        this->addGameObject(tmpSLight);

        // tmpLight->GetModule<dawn_engine::DirectionalLightModule>()->setAmbientColor(glm::vec3(1.0f, 0.0f, 0.0));
        // tmpLight->GetModule<dawn_engine::DirectionalLightModule>()->setDiffuseColor(glm::vec3(1.0f, 0.0f, 0.0));
    }

    void HelloWorldApp::update() {


    }
} // namespace helloworld