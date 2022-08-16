#pragma once

#include "common_includes.h"
#include "modules/light_module.h"
#include "modules/renderer_module.h"
#include "utils/resource_loader.h"

namespace dawn_engine {

    class DawnEngine;

    /**
     *
     */
    class GameObject {
    protected:
        static uint32_t nextGameObjectID;
        bool isEntity = true;
        GameObject *parent_ = nullptr ;
        std::vector<GameObject *> children_ {};
        std::string name = "New Game Object";
        uint32_t id;
        std::unordered_map<std::string, std::vector<BaseModule *>> module_dict_;

        void initGameObject();

    public:
        /**
         * Creates a game object with attached light module.
         * The properties of the light module is defined by the light type.
         * If the input light type is unknown, return nullptr.
         * @param light_type
         * @return The returned object is owned by DawnEngine and will be freed when engine shuts down.
         */
        static GameObject *CreateLight(LightType light_type);


        static GameObject *CreateSkybox(std::vector<std::string> faces_paths);

        static GameObject *CreateLine(glm::vec3 start_pos, glm::vec3 end_pos);

        GameObject();

        explicit GameObject(bool isEntity);

        GameObject(std::string name, bool isEntity);

        ~GameObject();
        int GetID() const;
        std::string GetName() const;

        void SetName(std::string objName);

        void SetParent(GameObject *parent);

        GameObject *GetParent() const;;

        unsigned int GetChildrenCount() const;

        GameObject * GetChild(unsigned int idx) const;

        bool IsTopLevel() const;

        std::unordered_map<std::string, std::vector<BaseModule *>> GetAllModules() const;

        uint32_t getModuleNum() const;

        template<class ModuleType>
        ModuleType *GetModule() const;

        template<class ModuleType, typename... Args>
        void AddModule(Args &&...params);

    };

    template<class ModuleType>
    ModuleType *GameObject::GetModule() const {
        if (!this->module_dict_.count(ModuleType::GetModuleType()) or
            this->module_dict_.at(ModuleType::GetModuleType()).empty()) {
            return static_cast<ModuleType *>(nullptr);
        }
        return static_cast<ModuleType *>(this->module_dict_.at(ModuleType::GetModuleType()).front());
    }

    template<class ModuleType, typename... Args>
    void GameObject::AddModule(Args &&...params) {
        if (!this->module_dict_.count(ModuleType::GetModuleType())) {
            this->module_dict_.insert({ModuleType::GetModuleType(), std::vector<BaseModule *>()});
        }
        auto *module_ptr = new ModuleType(std::forward<Args>(params)...);
        if (module_ptr->IsUnique() and !this->module_dict_.at(ModuleType::GetModuleType()).empty()) {
            throw std::runtime_error(
                    fmt::format("[GameObject:AddModule] {} is unique module that only one can exist in a game object",
                                ModuleType::GetModuleType()));
        }
        module_ptr->SetAttachedGameObject(this);
        this->module_dict_.at(ModuleType::GetModuleType()).emplace_back(module_ptr);
    }


} // namespace dawn_engine