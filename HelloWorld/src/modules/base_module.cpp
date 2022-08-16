#include "base_module.h"

namespace dawn_engine {
    BaseModule::BaseModule() = default;

    BaseModule::~BaseModule() = default;

    std::size_t BaseModule::GetHashModuleName(const char *moduleType) { return std::hash<std::string>()(moduleType); }

    GameObject *BaseModule::GetAttachedGameObject() const { return this->owner_ptr_; }

    void BaseModule::SetAttachedGameObject(GameObject *owner_ptr) { this->owner_ptr_ = owner_ptr; }

    bool BaseModule::GetActivation() const {
        return this->activation_;
    }

    bool &BaseModule::GetActivationMeta() {
        return this->activation_;
    }

    void BaseModule::SetActivation(const bool &active) {
        this->activation_ = active;
    }

    bool BaseModule::IsUnique() const {
        return this->is_unique_;
    }

    std::string BaseModule::GetModuleType() {
        return quote(BaseModule);
    }


} // namespace dawn_engine