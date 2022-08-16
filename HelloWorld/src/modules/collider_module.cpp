//
// Created by zhiquan on 7/7/22.
//

#include "collider_module.h"
#include "utils/collision_detection.h"


namespace dawn_engine {
    RayHitInfo ColliderModule::CollisionDetection(Ray ray) {
        return RayHitInfo();
    }

    ColliderBox ColliderModule::GetColliderShape() const {
        return this->collider_box_;
    }

    ColliderModule::ColliderModule(ColliderBox collider_shape) {

    }

    std::string ColliderModule::GetModuleType() {
        return quote(ModuleType);
    }


}
