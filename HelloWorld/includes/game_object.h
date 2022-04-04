#pragma once
#include "common_includes.h"
#include "module.h"
namespace dawn {

class GameObject {
  protected:
    static uint32_t nextGameObjectID;
    bool isEntity;
    std::unordered_map<std::size_t, std::vector<std::shared_ptr<Module>>> moduleDict;
    void initModuleDict();

  public:
    static std::shared_ptr<GameObject> createPrimitive(PrimitiveType pType);

    GameObject();
    GameObject(bool isEntity);
    ~GameObject();
    // void addModule(ModuleType mType);
    template <class ModuleType> ModuleType *getModule() const;
    // std::vector<std::shared_ptr<Module>> getModules(ModuleType mType);
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
        this->moduleDict.insert({ModuleType::type, std::vector<std::shared_ptr<Module>>()});
    }
    this->moduleDict.at(ModuleType::type).emplace_back(std::make_shared<ModuleType>(std::forward<Args>(params)...));
}

} // namespace dawn