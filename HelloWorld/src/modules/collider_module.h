//
// Created by zhiquan on 7/7/22.
//

#ifndef HELLOWORLD_COLLIDER_MODULE_H
#define HELLOWORLD_COLLIDER_MODULE_H

#include "base_module.h"
#include "utils/collision_detection.h"

namespace dawn_engine {
    enum ColliderType {
        MeshCollider, BoxCollider
    };

    class ColliderModule : public BaseModule {
    private:
        ColliderBox collider_box_;
    public:
        static const std::size_t type;

        ColliderModule(std::vector<std::shared_ptr<ColliderFace>> collider_faces);

        ColliderModule(ColliderBox collider_shape);

        RayHitInfo CollisionDetection(Ray ray);

        ColliderBox GetColliderShape() const;
    };


}


#endif //HELLOWORLD_COLLIDER_MODULE_H
