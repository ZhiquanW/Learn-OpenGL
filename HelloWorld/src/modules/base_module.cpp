#include "base_module.h"

namespace dawn_engine {
    BaseModule::BaseModule() = default;

    BaseModule::~BaseModule() = default;

    std::size_t BaseModule::getHashModuleName(const char *moduleType) { return std::hash<std::string>()(moduleType); }

    GameObject *BaseModule::GetAttachedGameObject() const { return this->owner_ptr_; }

    void BaseModule::SetAttachedGameObject(GameObject *owner_ptr) { this->owner_ptr_ = owner_ptr; }

    bool BaseModule::GetActivation() const {
        return this->activation;
    }

    bool &BaseModule::GetActivationMeta() {
        return this->activation;
    }

    void BaseModule::setActivation(const bool &active) {
        this->activation = active;

    }


} // namespace dawn_engine