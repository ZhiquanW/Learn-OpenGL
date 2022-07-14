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

//    void HelloWorldApp::AddDefaultLight() {
//        // std::shared_ptr<dawn_engine::PointLight> pLight(
//        //     new dawn_engine::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
//        // this->addLight(pLight);
//    }
//
//    void HelloWorldApp::addDefaultCube() {
//        // std::shared_ptr<dawn_engine::PointLight> pLight(
//        //     new dawn_engine::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
//        // this->addLight(pLight);
//        // this->AddGameObject(dawn_engine::GameObject::CreatePrimitive(dawn_engine::BoxPrimitive));
//        // this->AddGameObject(->gameObjects.emplace_back(GameObject::CreatePrimitive(BoxPrimitive));
//    }

    void HelloWorldApp::awake() {

    }

    void HelloWorldApp::start() {
        auto v0 = glm::vec3(1, 2, 3);
        auto v1 = glm::vec3(1.5, 2.5, 1.5);
        std::cout << glm::to_string(glm::min(v0, v1)) << std::endl;

        // std::shared_ptr<dawn_engine::PointLight> pLight(
        //     new dawn_engine::PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
        // this->addLight(pLight);
        // this->AddGameObject(dawn_engine::GameObject::CreatePrimitive(dawn_engine::BoxPrimitive));
        // load resource



        // tmpLight->GetModule<dawn_engine::DirectionalLightModule>()->SetAmbientColor(glm::vec3(1.0f, 0.0f, 0.0));
        // tmpLight->GetModule<dawn_engine::DirectionalLightModule>()->setDiffuseColor(glm::vec3(1.0f, 0.0f, 0.0));
        this->AddDefaultLight();
        this->AddDemoObjs();
    }

    void HelloWorldApp::update() {


    }

    void HelloWorldApp::AddDemoObjs() {
        auto skyboxObj = dawn_engine::GameObject::CreateSkybox({
                                                                       "../assets/skybox/right.jpg", "../assets/skybox/left.jpg",
                                                                       "../assets/skybox/top.jpg", "../assets/skybox/bottom.jpg",
                                                                       "../assets/skybox/front.jpg", "../assets/skybox/back.jpg",
                                                               });
        this->AddSkybox(skyboxObj);

        auto *game_obj = new dawn_engine::GameObject("Ray Line");
        std::vector<dawn_engine::DawnVertex> vertices = {dawn_engine::DawnVertex(glm::vec3(0.0f)), dawn_engine::DawnVertex(glm::vec3(0.0f, 1.0f, 0.0f))};
        auto line_mesh = dawn_engine::DawnMesh(vertices, {0, 1}, dawn_engine::DawnMaterial(glm::vec3(1.0f, 1.0f, 0.0f)));
        game_obj->AddModule<dawn_engine::RendererModule>(dawn_engine::DawnModel({line_mesh}));
        dawn_engine::DawnEngine::instance->AddGameObject(game_obj);
        auto box = dawn_engine::GameObject::CreatePrimitive(dawn_engine::BoxPrimitive);
        box->AddModule<dawn_engine::ColliderModule>(ColliderBox(box->GetModule<dawn_engine::TransformModule>()->GetPosition(), glm::vec3(1.0f)));
        dawn_engine::DawnEngine::instance->AddGameObject(box);

//        dawn_engine::DawnModel backpackModel = dawn_engine::DawnModel("../assets/backpack/backpack.obj");
//        auto backpack_obj = new dawn_engine::GameObject();
//        backpack_obj->AddModule<dawn_engine::RendererModule>(backpackModel);
//        this->AddGameObject(backpack_obj);
////         add game object
//
//        auto opaque_cube = dawn_engine::GameObject::CreatePrimitive(dawn_engine::BoxPrimitive);
//        opaque_cube->GetModule<dawn_engine::TransformModule>()->SetPosition(
//                glm::vec3(2.0, 0.0, 0.0));
//        this->AddGameObject(opaque_cube);
//        auto transparent_cube = dawn_engine::GameObject::CreatePrimitive(dawn_engine::BoxPrimitive);
//        transparent_cube->GetModule<dawn_engine::TransformModule>()->SetPosition(
//                glm::vec3(-2.0, 0.0, 0.0));
//        transparent_cube->GetModule<dawn_engine::RendererModule>()->GetModelRef().EnableAllTransparent();
//        transparent_cube->GetModule<dawn_engine::RendererModule>()->GetModelRef().SetAllTransparent(0.5f);
//        this->AddGameObject(transparent_cube);
//
//        auto tmpPLight = dawn_engine::GameObject::CreateLight(
//                dawn_engine::LightType::PointLightType);
//        tmpPLight->GetModule<dawn_engine::TransformModule>()->SetPosition(glm::vec3(0, 0, 1));
//        this->AddGameObject(tmpPLight);
//        auto tmpSLight = dawn_engine::GameObject::CreateLight(
//                dawn_engine::LightType::SpotLightType);
//        tmpSLight->GetModule<dawn_engine::TransformModule>()->SetPosition(
//                glm::vec3(0, 0, 1));
//        tmpSLight->GetModule<dawn_engine::SpotLightModule>()->setDirection(
//                glm::vec3(0.0, 0, -1));
//        this->AddGameObject(tmpSLight);

    }

    void HelloWorldApp::AddDefaultLight() {
        auto tmpLight = dawn_engine::GameObject::CreateLight(
                dawn_engine::LightType::DirectionalLightType);
        tmpLight->GetModule<dawn_engine::DirectionalLightModule>()->setDirection(
                glm::vec3(-1.0, -1.0, -1.0));
        tmpLight->GetModule<dawn_engine::DirectionalLightModule>()->SetAmbient(glm::vec3(0.4f));
        this->AddGameObject(tmpLight);
    }


} // namespace helloworld