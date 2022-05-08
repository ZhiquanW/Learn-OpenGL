#pragma once
#include "common_includes.h"
#include "material.h"
namespace dawn_engine {

enum PrimitiveType { CubePrimitiveType, SpherePrimitiveType };
enum LightType { DirectionalLightType, PointLightType };

class GameObject;
class BaseModule {

  protected:
    static const std::size_t type;
    std::shared_ptr<GameObject> gameObjectPtr;

  public:
    friend class GameObject;
    BaseModule();
    virtual ~BaseModule(){};
    virtual std::shared_ptr<GameObject> getAttachedGameObjet() const;
    static const std::size_t getHashModuleName(const char *moduleType);
};

} // namespace dawn_engine