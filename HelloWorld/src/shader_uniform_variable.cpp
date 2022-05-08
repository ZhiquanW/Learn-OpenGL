#include "shader_uniform_variable.h"

namespace dawn_engine {
std::string ShaderUniformVariableBase::getName() const { return this->name; }
std::size_t ShaderUniformVariableBase::getTypeHash() const { return this->type_hash; }

} // namespace dawn_engine