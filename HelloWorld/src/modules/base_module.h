#pragma once

#include "common_includes.h"
#include "material.h"

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
        GameObject *ownerPtr{};
        bool activation = true;
    public:
        static const std::size_t type;

        friend class GameObject;

        BaseModule();

        virtual ~BaseModule() = 0;

        bool getActivation() const;

        bool &getActivationMeta();

        void setActivation(const bool &active);

        [[nodiscard]] GameObject *getAttachedGameObject() const;

        void setAttachedGameObject(GameObject *ownerPtr);

        static std::size_t getHashModuleName(const char *moduleType);
    };

} // namespace dawn_engine