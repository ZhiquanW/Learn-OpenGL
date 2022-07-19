#pragma once

#include "common_includes.h"
#include "modules/transform_module.h"
#include "modules/renderer_module.h"
//#include "core/dawn_engine.h"
#include "modules/light_module.h"
#include "utils/resource_loader.h"

namespace dawn_engine {
    class GameObject {
    protected:
        static uint32_t nextGameObjectID;
        bool isEntity = true;
        std::string name = "New Game Object";
        uint32_t id;
        std::unordered_map<std::size_t, std::vector<BaseModule *>> moduleDict;

        void initGameObject();

    public:

        static GameObject *CreateLight(LightType lType);

        static GameObject *CreateSkybox(std::vector<std::string> facesPaths);

        static GameObject *CreateLine(glm::vec3 start_pos, glm::vec3 end_pos);

        GameObject();

        explicit GameObject(bool isEntity);

        GameObject(std::string name, bool isEntity);

        ~GameObject();

        std::string GetName() const;

        void SetName(std::string objName);

        std::unordered_map<std::size_t, std::vector<BaseModule *>> getModules() const;

        uint32_t getModuleNum() const;

        template<class ModuleType>
        ModuleType *GetModule() const;

        template<class ModuleType, typename... Args>
        void AddModule(Args &&...params);

    };

    template<class ModuleType>
    ModuleType *GameObject::GetModule() const {
        if (!this->moduleDict.count(ModuleType::type) or
            this->moduleDict.at(ModuleType::type).empty()) {
            return static_cast<ModuleType *>(nullptr);
        }
        return static_cast<ModuleType *>(this->moduleDict.at(ModuleType::type).front());
    }

    template<class ModuleType, typename... Args>
    void GameObject::AddModule(Args &&...params) {
        if (!this->moduleDict.count(ModuleType::type)) {
            this->moduleDict.insert({ModuleType::type, std::vector<BaseModule *>()});
        }
        auto *module_ptr = new ModuleType(std::forward<Args>(params)...);
        module_ptr->SetAttachedGameObject(this);
        this->moduleDict.at(ModuleType::type).emplace_back(module_ptr);
    }


} // namespace dawn_engine