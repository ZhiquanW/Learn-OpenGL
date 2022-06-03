#include "game_object.h"

#include <utility>

namespace dawn_engine {
    uint32_t GameObject::nextGameObjectID = 0;

    GameObject::GameObject() : isEntity(false),
                               name("New GameObject") { this->initGameObject(); }

    GameObject::GameObject(bool isEntity) : isEntity(isEntity), name("New GameObject") {
        this->initGameObject();
        if (this->isEntity) {
            this->addModule<TransformModule>();
        }
    }

    GameObject::GameObject(std::string name, bool isEntity) : name(std::move(name)),
                                                              isEntity(isEntity) {
        this->initGameObject();
        if (this->isEntity) {
            this->addModule<TransformModule>();
        }
    }

    void GameObject::initGameObject() {
        this->id = nextGameObjectID;
        nextGameObjectID += 1;
        this->moduleDict = {};
    }

    GameObject::~GameObject() = default;

    uint32_t GameObject::getModuleNum() const {
        uint32_t module_num = 0;
        for (std::pair<std::size_t, std::vector<BaseModule *>> element: this->moduleDict) {
            module_num += element.second.size();
        }
        return module_num;
    }

    GameObject *GameObject::createPrimitive(PrimitiveType pType) {
        switch (pType) {
            case CubePrimitiveType: {
                float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f,
                                    0.0f, 0.0f, -1.0f,
                                    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, 0.5f, -0.5f,
                                    0.0f, 0.0f, -1.0f,
                                    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, -0.5f, -0.5f,
                                    0.0f, 0.0f, -1.0f,

                                    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f,
                                    0.0f, 0.0f, 1.0f,
                                    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0.0f,
                                    0.0f, 1.0f,
                                    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,
                                    0.0f, 0.0f, 1.0f,

                                    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f,
                                    -1.0f, 0.0f, 0.0f,
                                    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -0.5f,
                                    -1.0f, 0.0f, 0.0f,
                                    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f,
                                    -1.0f, 0.0f, 0.0f,

                                    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f,
                                    0.0f, 0.0f,
                                    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f,
                                    1.0f, 0.0f, 0.0f,
                                    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f,
                                    0.0f, 0.0f,

                                    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f,
                                    0.0f, -1.0f, 0.0f,
                                    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, 0.5f,
                                    0.0f, -1.0f, 0.0f,
                                    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, -0.5f,
                                    0.0f, -1.0f, 0.0f,

                                    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f,
                                    0.0f, 1.0f, 0.0f,
                                    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f,
                                    1.0f, 0.0f,
                                    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f,
                                    0.0f, 1.0f, 0.0f};

                auto *cubePrimitive(new GameObject("New Cube Primitive", true));
                cubePrimitive->addModule<MeshModule>(
                        std::vector<float>(vertices, vertices + 216));
                return cubePrimitive;
                break;
            }

            default:
                break;
        }
        return nullptr;
    }

    GameObject *GameObject::createLight(LightType lType) {
        switch (lType) {
            case DirectionalLightType: {
                auto *lightObj(new GameObject("New Directional Light", true));
                lightObj->addModule<DirectionalLightModule>(glm::vec3(1, 0, 0));
                return lightObj;
                break;
            }
            case PointLightType: {
                auto *lightObj(new GameObject("New Point Light", true));
                lightObj->addModule<PointLightModule>(glm::vec3(0.2f, 0.5, 0.5f), 1.0, 0.1,
                                                      0.08);
                return lightObj;
                break;
            }
            case SpotLightType: {
                auto *lightObj(new GameObject("New Spot Light", true));
                lightObj->addModule<SpotLightModule>(glm::vec3(1.0, 0, 0), 15, 30);
                return lightObj;
                break;
            }
            default:
                break;
        }
        return nullptr;
    }

    std::string GameObject::getName() const {
        return this->name;
    }

    void GameObject::setName(std::string objName) {
        this->name = std::move(objName);
    }

    std::unordered_map<std::size_t, std::vector<BaseModule *>> GameObject::getModules() const {
        return this->moduleDict;
    }

} // namespace dawn_engine