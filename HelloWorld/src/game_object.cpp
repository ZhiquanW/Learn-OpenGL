#include "game_object.h"

#include <utility>

namespace dawn_engine {
    uint32_t GameObject::nextGameObjectID = 0;

    GameObject::GameObject() : id(nextGameObjectID) {
        this->initGameObject();
    }

    GameObject::GameObject(bool isEntity) : id(nextGameObjectID), isEntity(isEntity) {
        this->initGameObject();
    }

    GameObject::GameObject(std::string name, bool isEntity) : name(std::move(name)),
                                                              isEntity(isEntity) {
        this->initGameObject();
    }

    void GameObject::initGameObject() {
        nextGameObjectID += 1;
        this->moduleDict = {};
        if (this->isEntity) {
            this->addModule<TransformModule>();
        }
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
        struct TVertex {
            glm::vec3 position{};
            glm::vec3 normal{};
            glm::vec2 texCoords{};
        };
        switch (pType) {
            case CubePrimitiveType: {
                std::vector<float> positionRaw = {-0.5f, -0.5f, -0.5f,
                                                  0.5f, -0.5f, -0.5f,
                                                  0.5f, 0.5f, -0.5f,
                                                  0.5f, 0.5f, -0.5f,
                                                  -0.5f, 0.5f, -0.5f,
                                                  -0.5f, -0.5f, -0.5f,

                                                  -0.5f, -0.5f, 0.5f,
                                                  0.5f, -0.5f, 0.5f,
                                                  0.5f, 0.5f, 0.5f,
                                                  0.5f, 0.5f, 0.5f,
                                                  -0.5f, 0.5f, 0.5f,
                                                  -0.5f, -0.5f, 0.5f,

                                                  -0.5f, 0.5f, 0.5f,
                                                  -0.5f, 0.5f, -0.5f,
                                                  -0.5f, -0.5f, -0.5f,
                                                  -0.5f, -0.5f, -0.5f,
                                                  -0.5f, -0.5f, 0.5f,
                                                  -0.5f, 0.5f, 0.5f,


                                                  0.5f, 0.5f, 0.5f,
                                                  0.5f, 0.5f, -0.5f,
                                                  0.5f, -0.5f, -0.5f,
                                                  0.5f, -0.5f, -0.5f,
                                                  0.5f, -0.5f, 0.5f,
                                                  0.5f, 0.5f, 0.5f,

                                                  -0.5f, -0.5f, -0.5f,
                                                  0.5f, -0.5f, -0.5f,
                                                  0.5f, -0.5f, 0.5f,
                                                  0.5f, -0.5f, 0.5f,
                                                  -0.5f, -0.5f, 0.5f,
                                                  -0.5f, -0.5f, -0.5f,

                                                  -0.5f, 0.5f, -0.5f,
                                                  0.5f, 0.5f, -0.5f,
                                                  0.5f, 0.5f, 0.5f,
                                                  0.5f, 0.5f, 0.5f,
                                                  -0.5f, 0.5f, 0.5f,
                                                  -0.5f, 0.5f, -0.5f};
                std::vector<float> normalRaw = {0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f};
                std::vector<float> texCoordsRaw = {0.0f, 0.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 0.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 1.0f,
                                                   0.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 1.0f,
                                                   0.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   1.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 0.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 0.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f};
                int num_vertices = 36;  // 6 faces * 6 vertex on each face (2 triangles on each face)
                std::vector<Vertex> vertices;
                for (int i = 0; i < num_vertices; ++i) {
                    Vertex tmpV = {};
                    tmpV.position = glm::vec3(positionRaw[i * 3], positionRaw[i * 3 + 1], positionRaw[i * 3 + 2]);
                    tmpV.normal = glm::vec3(normalRaw[i * 3], normalRaw[i * 3 + 1], normalRaw[i * 3 + 2]);
                    tmpV.texCoords = glm::vec2(texCoordsRaw[i * 2], texCoordsRaw[i * 2 + 1]);
                    vertices.push_back(tmpV);
                }
                std::vector<unsigned int> indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                                                     16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
                DawnMesh mesh = DawnMesh(vertices, indices, DawnMaterial());
                auto *cubePrimitive(new GameObject("New Cube Primitive", true));
                cubePrimitive->addModule<MeshModule>((std::vector<DawnMesh>) {mesh});
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