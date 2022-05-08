#include "base_module.h"

namespace dawn_engine {
BaseModule::BaseModule() {}
const std::size_t BaseModule::getHashModuleName(const char *moduleType) { return std::hash<std::string>()(moduleType); }

std::shared_ptr<GameObject> BaseModule::getAttachedGameObjet() const { return this->gameObjectPtr; }

} // namespace dawn_engine