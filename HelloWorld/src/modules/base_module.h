#pragma once
#include "common_includes.h"
#include "material.h"
namespace dawn_engine {

enum PrimitiveType { CubePrimitiveType, SpherePrimitiveType };
enum LightType { DirectionalLightType, PointLightType };

class GameObject;
class BaseModule {

  protected:
    GameObject *ownerPtr;

  public:
    static const std::size_t type;
    friend class GameObject;
    BaseModule();
    virtual ~BaseModule() = 0;
    GameObject *getAttachedGameObject() const;
    void setAttachedGameObject(GameObject *ownerPtr);
    static const std::size_t getHashModuleName(const char *moduleType);
};

} // namespace dawn_engine