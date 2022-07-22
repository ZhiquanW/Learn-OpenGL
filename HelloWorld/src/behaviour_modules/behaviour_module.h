//
// Created by zhiquan on 7/21/22.
//

#ifndef HELLOWORLD_BEHAVIOUR_MODULE_H
#define HELLOWORLD_BEHAVIOUR_MODULE_H

//#include <utility>
#include "common_includes.h"
#include "modules/base_module.h"

namespace dawn_engine {
    class BehaviourModule : public BaseModule {
    protected:
        std::string name_;

        virtual void Awake() = 0;

        virtual void Start() = 0;

        virtual void Update() = 0;

        friend class DawnEngine;


    public:
        static const std::size_t type;

        explicit BehaviourModule(std::string name) : name_(std::move(name)) {}


        std::string GetName() const {
            return this->name_;
        }

        virtual void UpdateModuleMonitor() {}

    };


}


//class BaseModule {
//
//protected:
//    GameObject *owner_ptr_{};
//    bool activation = true;
//public:
//    static const std::size_t type;
//
//    friend class GameObject;
//
//    BaseModule();
//
//    virtual ~BaseModule() = 0;
//
//    bool GetActivation() const;
//
//    bool &GetActivationMeta();
//
//    void setActivation(const bool &active);
//
//    [[nodiscard]] GameObject *GetAttachedGameObject() const;
//
//    void SetAttachedGameObject(GameObject *owner_ptr);
//
//    static std::size_t GetHashModuleName(const char *moduleType);
//};
#endif //HELLOWORLD_BEHAVIOUR_MODULE_H
