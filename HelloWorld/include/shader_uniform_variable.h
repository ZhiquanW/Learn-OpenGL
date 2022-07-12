#pragma once

#include "common_includes.h"

namespace dawn_engine {
    class ShaderUniformVariableBase {
    protected:
        std::string name;
        std::size_t type_hash;

    public:

        ShaderUniformVariableBase();

        ShaderUniformVariableBase(const std::string &name) : name(name), type_hash(0) {}

        std::string getName() const;

        std::size_t GetTypeHash() const;

        virtual ~ShaderUniformVariableBase() {};

        static std::vector<long> GetUniformsDataSizes(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);

        static long GetUniformsDataSize(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms);

        static long GetUniformDataSize(const std::shared_ptr<ShaderUniformVariableBase> uniform);

        static long GetDataBaseAlignment(const std::shared_ptr<ShaderUniformVariableBase> &uniform);
    };

    template<typename DataType>
    class ShaderUniformVariable : public ShaderUniformVariableBase {
    private:
        DataType data;

    public:
        ShaderUniformVariable(const std::string &name, DataType data) : ShaderUniformVariableBase(name), data(data) {
            this->type_hash = typeid(DataType).hash_code();
        }

        DataType GetData() const { return this->data; }

        DataType &GetDataRef() { return this->data; }

        [[nodiscard]] long GetDataSize() const {
            return sizeof(this->data);
        }
    };
} // namespace dawn_engine