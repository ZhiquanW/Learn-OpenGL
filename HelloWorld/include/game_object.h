#pragma once

#include "common_includes.h"
#include "mesh_module.h"
#include "transform_module.h"
#include "dawn_engine.h"
#include "light_module.h"

namespace dawn_engine {
    class GameObject {
    protected:
        static uint32_t nextGameObjectID;
        std::string name;
        uint32_t id;
        bool isEntity;
        std::unordered_map<std::size_t, std::vector<BaseModule *>> moduleDict;

        void initGameObject();

    public:
        static GameObject *createPrimitive(PrimitiveType pType);

        static GameObject *createLight(LightType lType);

        GameObject();

        explicit GameObject(bool isEntity);

        GameObject(std::string name, bool isEntity);

        ~GameObject();

        std::string getName() const;

        void setName(std::string objName);

        std::unordered_map<std::size_t, std::vector<BaseModule *>> getModules() const;

        uint32_t getModuleNum() const;

        template<class ModuleType>
        ModuleType *getModule() const;

        template<class ModuleType, typename... Args>
        void addModule(Args &&...params);
    };

    template<class ModuleType>
    ModuleType *GameObject::getModule() const {
        if (!this->moduleDict.count(ModuleType::type) or
            this->moduleDict.at(ModuleType::type).empty()) {
            return static_cast<ModuleType *>(nullptr);
        }
        return static_cast<ModuleType *>(this->moduleDict.at(ModuleType::type).front());
    }

    template<class ModuleType, typename... Args>
    void GameObject::addModule(Args &&...params) {
        if (!this->moduleDict.count(ModuleType::type)) {
            this->moduleDict.insert({ModuleType::type, std::vector<BaseModule *>()});
        }
        auto *module_ptr = new ModuleType(std::forward<Args>(params)...);
        module_ptr->setAttachedGameObject(this);
        this->moduleDict.at(ModuleType::type).emplace_back(module_ptr);
    }

} // namespace dawn_engine