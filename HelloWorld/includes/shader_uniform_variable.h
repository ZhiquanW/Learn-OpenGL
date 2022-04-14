#pragma once
#include "common_includes.h"
namespace dawn {
class ShaderUniformVariableBase {
  protected:
    std::string name;
    std::size_t type_hash;

  public:
    ShaderUniformVariableBase();
    ShaderUniformVariableBase(const std::string &name) : name(name), type_hash(0) {}
    std::string getName() const;
    std::size_t getTypeHash() const;
    virtual ~ShaderUniformVariableBase(){};
};
template <typename DataType> class ShaderUniformVariable : public ShaderUniformVariableBase {
  private:
    DataType data;

  public:
    ShaderUniformVariable(const std::string &name, DataType data) : ShaderUniformVariableBase(name), data(data) {
        this->type_hash = typeid(DataType).hash_code();
    }
    DataType getData() const { return this->data; }
};
} // namespace dawn