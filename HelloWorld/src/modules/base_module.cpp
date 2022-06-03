#include "base_module.h"

namespace dawn_engine {
    BaseModule::BaseModule() = default;

    BaseModule::~BaseModule() = default;

    std::size_t BaseModule::getHashModuleName(const char *moduleType) { return std::hash<std::string>()(moduleType); }

    GameObject *BaseModule::getAttachedGameObject() const { return this->ownerPtr; }

    void BaseModule::setAttachedGameObject(GameObject *ownerPtr) { this->ownerPtr = ownerPtr; }

    bool BaseModule::getActivation() const {
        return this->activation;
    }

    bool &BaseModule::getActivationMeta() {
        return this->activation;
    }

    void BaseModule::setActivation(const bool &active) {
        this->activation = active;

    }


} // namespace dawn_engine