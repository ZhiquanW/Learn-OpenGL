#include "game_object.h"
#include <utility>
#include "dawn_engine.h"

namespace dawn_engine {
    uint32_t GameObject::nextGameObjectID = 0;

    GameObject::GameObject() : id(nextGameObjectID) {
        this->initGameObject();
    }

    GameObject::GameObject(bool isEntity) : id(nextGameObjectID), isEntity(isEntity) {
        this->initGameObject();
    }

    GameObject::GameObject(std::string name, bool isEntity) : id(nextGameObjectID), name(std::move(name)),
                                                              isEntity(isEntity) {
        this->initGameObject();
    }

    void GameObject::initGameObject() {
        nextGameObjectID += 1;
        this->moduleDict = {};
        if (this->isEntity) {
            this->AddModule<TransformModule>();
        }
    }

    GameObject::~GameObject() {
        for (auto dictPair: this->moduleDict) {
            for (auto module: dictPair.second) {
                delete module;
            }
        }

    };

    uint32_t GameObject::getModuleNum() const {
        uint32_t module_num = 0;
        for (std::pair<std::size_t, std::vector<BaseModule *>> element: this->moduleDict) {
            module_num += element.second.size();
        }
        return module_num;
    }

    GameObject *GameObject::CreatePrimitive(PrimitiveType pType) {
        switch (pType) {
            case BoxPrimitive: {
                std::vector<float> positionRaw = {
                        -0.5f, -0.5f, -0.5f, // face 1: left
                        -0.5f, -0.5f, 0.5f,
                        -0.5f, 0.5f, -0.5f,
                        -0.5f, 0.5f, 0.5f, // indices_: {0 1 2} {1 3 2}
                        0.5f, -0.5f, -0.5f, // face 2: right
                        0.5f, -0.5f, 0.5f,
                        0.5f, 0.5f, -0.5f,
                        0.5f, 0.5f, 0.5f, // indices_: {4 6 5} {5 6 7}
                        -0.5f, -0.5f, -0.5f, // face 3: bottom
                        -0.5f, -0.5f, 0.5f,
                        0.5f, -0.5f, -0.5f,
                        0.5f, -0.5f, 0.5f, // indices_: {8 10 9} {9 10 11}
                        -0.5f, 0.5f, -0.5f, // face 4: top
                        -0.5f, 0.5f, 0.5f,
                        0.5f, 0.5f, -0.5f,
                        0.5f, 0.5f, 0.5f, // indices_: {12 13 14} {13 15 14}
                        -0.5f, -0.5f, 0.5f, // face 5: front
                        -0.5f, 0.5f, 0.5f,
                        0.5f, -0.5f, 0.5f,
                        0.5f, 0.5f, 0.5f, // indices_: {16 18 17} {17 18 19}
                        -0.5f, -0.5f, -0.5f, // face 6: back
                        -0.5f, 0.5f, -0.5f,
                        0.5f, -0.5f, -0.5f,
                        0.5f, 0.5f, -0.5f, // indices_: {20 21 22} {21 23 22}
                };
                std::vector<float> normalRaw = {-1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                -1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, -1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                };
                std::vector<float> texCoordsRaw = {0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f,
                                                   0.0f, 0.0f,
                                                   0.0f, 1.0f,
                                                   1.0f, 0.0f,
                                                   1.0f, 1.0f};
                int num_vertices = 24;  // 6 faces * 4 vertex on each face (2 triangles on each face)
                std::vector<DawnVertex> vertices;
                for (int i = 0; i < num_vertices; ++i) {
                    DawnVertex tmpV = {};
                    tmpV.position = glm::vec3(positionRaw[i * 3], positionRaw[i * 3 + 1], positionRaw[i * 3 + 2]);
                    tmpV.normal = glm::vec3(normalRaw[i * 3], normalRaw[i * 3 + 1], normalRaw[i * 3 + 2]);
                    tmpV.texCoords = glm::vec2(texCoordsRaw[i * 2], texCoordsRaw[i * 2 + 1]);
                    vertices.push_back(tmpV);
                }
                std::vector<unsigned int> indices = {0, 1, 2, 1, 3, 2,
                                                     4, 6, 5, 5, 6, 7,
                                                     8, 10, 9, 9, 10, 11,
                                                     12, 13, 14, 13, 15, 14,
                                                     16, 18, 17, 17, 18, 19,
                                                     20, 21, 22, 21, 23, 22
                };
                DawnMesh mesh = DawnMesh(vertices, indices, DawnMaterial());
                auto *cubePrimitive(new GameObject("New Cube Primitive", true));
                DawnModel model({mesh});
                cubePrimitive->AddModule<RendererModule>(model);
                return cubePrimitive;
                break;
            }
            default:
                break;
        }
        return nullptr;
    }

    GameObject *GameObject::CreateSkybox(std::vector<std::string> facesPaths) {
        std::vector<float> positionRaw = {
                // positions
                -1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,

                -1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f
        };
        int num_vertices = 36;  // 6 faces * (3*2) vertex on each face (2 triangles on each face)
        std::vector<DawnVertex> vertices;
        for (int i = 0; i < num_vertices; ++i) {
            DawnVertex tmp_v = {};
            tmp_v.position = glm::vec3(positionRaw[i * 3], positionRaw[i * 3 + 1], positionRaw[i * 3 + 2]);
            tmp_v.normal = glm::vec3(0.0f);
            tmp_v.texCoords = glm::vec2(0.0f);
            vertices.push_back(tmp_v);
        }
        std::vector<unsigned int> indices = {
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
        };
        std::vector<std::shared_ptr<DawnTexture>> cube_map_textures = {};
        for (const auto &path: facesPaths) {
            cube_map_textures.emplace_back(std::make_shared<DawnTexture>(path));
        }
        DawnMaterial skybox_material = DawnMaterial(cube_map_textures);
        DawnMesh mesh = DawnMesh(vertices, indices, skybox_material);
        DawnModel model = DawnModel({mesh});
        auto *skybox(new GameObject("New Skybox", true));
        skybox->AddModule<RendererModule>(model);
        return skybox;
    }

    GameObject *GameObject::CreateLight(LightType lType) {
        switch (lType) {
            case DirectionalLightType: {
                auto *lightObj(new GameObject("New Directional Light", true));
                lightObj->AddModule<DirectionalLightModule>(glm::vec3(1, 0, 0));
                return lightObj;
                break;
            }
            case PointLightType: {
                auto *lightObj(new GameObject("New Point Light", true));
                lightObj->AddModule<PointLightModule>(glm::vec3(0.2f, 0.5, 0.5f), 1.0, 0.1,
                                                      0.08);
                return lightObj;
                break;
            }
            case SpotLightType: {
                auto *lightObj(new GameObject("New Spot Light", true));
                lightObj->AddModule<SpotLightModule>(glm::vec3(1.0, 0, 0), 15, 30);
                return lightObj;
                break;
            }
            default:
                break;
        }
        return nullptr;
    }

    GameObject *GameObject::CreateLine(glm::vec3 start_pos, glm::vec3 end_pos) {
        return nullptr;
    }

    std::string GameObject::GetName() const {
        return this->name;
    }

    void GameObject::setName(std::string objName) {
        this->name = std::move(objName);
    }

    std::unordered_map<std::size_t, std::vector<BaseModule *>> GameObject::getModules() const {
        return this->moduleDict;
    }

} // namespace dawn_engine