#include "base_module.h"

namespace dawn_engine {
BaseModule::BaseModule() {}
BaseModule::~BaseModule() {}
const std::size_t BaseModule::getHashModuleName(const char *moduleType) { return std::hash<std::string>()(moduleType); }

GameObject *BaseModule::getAttachedGameObject() const { return this->ownerPtr; }
void BaseModule::setAttachedGameObject(GameObject *ownerPtr) { this->ownerPtr = ownerPtr; }

} // namespace dawn_engine