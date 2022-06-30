#pragma once

#include "../../include/common_includes.h"

namespace dawn_engine {

    enum PrimitiveType {
        CubePrimitiveType, SpherePrimitiveType
    };
    enum LightType {
        DirectionalLightType, PointLightType, SpotLightType
    };

    class GameObject;

    class BaseModule {

    protected:
        GameObject *owner_ptr_{};
        bool activation = true;
    public:
        static const std::size_t type;

        friend class GameObject;

        BaseModule();

        virtual ~BaseModule() = 0;

        bool getActivation() const;

        bool &getActivationMeta();

        void setActivation(const bool &active);

        [[nodiscard]] GameObject *GetAttachedGameObject() const;

        void SetAttachedGameObject(GameObject *owner_ptr);

        static std::size_t getHashModuleName(const char *moduleType);
    };

} // namespace dawn_engine