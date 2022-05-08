#pragma once
#include "common_includes.h"
#include "mesh_module.h"
#include "transform_module.h"
#include "dawn_engine.h"
namespace dawn_engine {
class GameObject {
  protected:
    static uint32_t nextGameObjectID;
    std::unordered_map<std::size_t, std::vector<std::shared_ptr<BaseModule>>> moduleDict;
    void initModuleDict();
    void initGameObject();

  public:
    static std::shared_ptr<GameObject> createPrimitive(PrimitiveType pType);

    static std::shared_ptr<GameObject> createLight(LightType lType);

    GameObject();
    GameObject(bool isEntity);
    ~GameObject();
    bool isEntity;
    uint32_t getModuleNum() const;
    template <class ModuleType> ModuleType *getModule() const;
    template <class ComponentType, typename... Args> void addModule(Args &&...params);
};
template <class ModuleType> ModuleType *GameObject::getModule() const {
    if (!this->moduleDict.count(ModuleType::type) or this->moduleDict.at(ModuleType::type).empty()) {
        return static_cast<ModuleType *>(nullptr);
    }
    return static_cast<ModuleType *>(this->moduleDict.at(ModuleType::type).front().get());
}
template <class ModuleType, typename... Args> void GameObject::addModule(Args &&...params) {
    if (!this->moduleDict.count(ModuleType::type)) {
        this->moduleDict.insert({ModuleType::type, std::vector<std::shared_ptr<BaseModule>>()});
    }
    this->moduleDict.at(ModuleType::type).emplace_back(std::make_shared<ModuleType>(std::forward<Args>(params)...));
}

} // namespace dawn_engine