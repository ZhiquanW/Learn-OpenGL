#pragma once

#include "../../include/common_includes.h"

namespace dawn_engine {

    enum PrimitiveType {
        BoxPrimitive, SpherePrimitiveType
    };

    class GameObject;

    class BaseModule {

    protected:
        GameObject *owner_ptr_{};
        bool activation_ = true;
        bool is_unique_ = true;
    public:

        BaseModule();

        virtual ~BaseModule() = 0;

        bool GetActivation() const;

        bool &GetActivationMeta();

        void SetActivation(const bool &active);

        [[nodiscard]] GameObject *GetAttachedGameObject() const;

        void SetAttachedGameObject(GameObject *owner_ptr);

        static std::size_t GetHashModuleName(const char *moduleType);

        bool IsUnique() const;

        static std::string GetModuleType();
    };

} // namespace dawn_engine