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
        bool activation = true;
    public:
        static const std::size_t type;


        BaseModule();

        virtual ~BaseModule() = 0;

        bool GetActivation() const;

        bool &GetActivationMeta();

        void setActivation(const bool &active);

        [[nodiscard]] GameObject *GetAttachedGameObject() const;

        void SetAttachedGameObject(GameObject *owner_ptr);

        static std::size_t GetHashModuleName(const char *moduleType);
    };

} // namespace dawn_engine