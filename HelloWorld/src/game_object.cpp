#include "game_object.h"

namespace dawn_engine {
uint32_t GameObject::nextGameObjectID = 0;

GameObject::GameObject() : isEntity(false) { this->initGameObject(); }
GameObject::GameObject(bool isEntity) : isEntity(isEntity) {
    this->initGameObject();
    if (this->isEntity) {
        this->addModule<TransformModule>();
    }
}

void GameObject::initGameObject() {
    nextGameObjectID += 1;
    this->moduleDict = {};
}
GameObject::~GameObject() { nextGameObjectID -= 1; }

uint32_t GameObject::getModuleNum() const {
    uint32_t module_num = 0;
    for (std::pair<std::size_t, std::vector<std::shared_ptr<BaseModule>>> element : this->moduleDict) {
        module_num += element.second.size();
    }
    return module_num;
}
std::shared_ptr<GameObject> GameObject::createPrimitive(PrimitiveType pType) {
    switch (pType) {
    case CubePrimitiveType: {
        float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,
                            0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
                            -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

                            -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,
                            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                            -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

                            -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,
                            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
                            -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

                            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,
                            0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
                            0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                            -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
                            0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

                            -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,
                            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                            -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

        std::shared_ptr<GameObject> cubePrimitive(new GameObject(true));
        cubePrimitive->addModule<MeshModule>(std::vector<float>(vertices, vertices + 216));
        return cubePrimitive;
        break;
    }

    default:
        break;
    }
    return nullptr;
}
std::shared_ptr<GameObject> GameObject::createLight(LightType lType) {
    switch (lType) {
    case DirectionalLightType:
        /* code */
        break;

    default:
        break;
    }
}

} // namespace dawn_engine