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
        this->module_dict_ = {};
        if (this->isEntity) {
            this->AddModule<TransformModule>();
        }
        DawnEngine::instance->AddGameObject(this);

    }

    GameObject::~GameObject() {
        for (auto dictPair: this->module_dict_) {
            for (auto module: dictPair.second) {
                delete module;
            }
        }

    };

    uint32_t GameObject::getModuleNum() const {
        uint32_t module_num = 0;
        for (std::pair<std::string, std::vector<BaseModule *>> element: this->module_dict_) {
            module_num += element.second.size();
        }
        return module_num;
    }



    GameObject *GameObject::CreateSkybox(std::vector<std::string> faces_paths) {
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
        for (const auto &path: faces_paths) {
            cube_map_textures.emplace_back(std::make_shared<DawnTexture>(path));
        }
        auto  skybox_material = std::make_shared<DawnMaterial>(cube_map_textures);
        DawnMesh mesh = DawnMesh(vertices, indices, skybox_material);
        DawnModel model = DawnModel({mesh});
        auto *skybox(new GameObject("New Skybox", true));
        skybox->AddModule<RendererModule>(model);
        return skybox;
    }

    GameObject *GameObject::CreateLight(LightType light_type) {
        switch (light_type) {
            case DirectionalLight: {
                auto *lightObj(new GameObject("New Directional Light", true));
                lightObj->AddModule<DirectionalLightModule>(glm::vec3(1, 0, 0));
                return lightObj;
                break;
            }
            case PointLight: {
                auto *lightObj(new GameObject("New Point Light", true));
                lightObj->AddModule<PointLightModule>(glm::vec3(0.2f, 0.5, 0.5f), 1.0, 0.1,
                                                      0.08);
                return lightObj;
                break;
            }
            case SpotLight: {
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

    void GameObject::SetName(std::string objName) {
        this->name = std::move(objName);
    }

    std::unordered_map<std::string, std::vector<BaseModule *>> GameObject::GetAllModules() const {
        return this->module_dict_;
    }

    /**
     * Set the parent game object ptr by input game object
     * Add this to parent game object's children
     * @param parent
     */
    void GameObject::SetParent(GameObject *parent) {
        this->parent_ = parent;
        parent->children_.push_back(this);
    }

    GameObject *GameObject::GetParent() const {
        return this->parent_;
    }

    unsigned int GameObject::GetChildrenCount() const {
        return this->children_.size();
    }

    GameObject *GameObject::GetChild(unsigned int idx) const {
        return this->children_[idx];
    }

    bool GameObject::IsTopLevel() const {
        return this->parent_ == nullptr;
    }

    int GameObject::GetID() const {
        return this->id;
    }

} // namespace dawn_engine