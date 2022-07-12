//
// Created by zhiquan on 7/7/22.
//

#ifndef HELLOWORLD_COLLIDER_MODULE_H
#define HELLOWORLD_COLLIDER_MODULE_H

#include "base_module.h"

namespace dawn_engine {
    class Ray {
    private:
        glm::vec3 start_pos_ = glm::vec3(0.0f);
        glm::vec3 direction_ = glm::vec3(1.0f, 0.0f, 0.0f);
    public:

        Ray() = default;

        Ray(glm::vec3 start_pos, glm::vec3 direction);

        glm::vec3 ToDirection(float length);

        [[nodiscard]] glm::vec3 GetStartPos() const;
    };

    class RayHitInfo {
    private:
    protected:
        RayHitInfo() = default;
    };

    enum ColliderType {
        MeshCollider, BoxCollider
    };

    class ColliderFace {
    protected:
    public:
        ColliderFace() = default;
    };

    class RectFace : public ColliderFace {
    protected:
        std::vector<glm::vec3> faces_ = std::vector<glm::vec3>(4);
    public:
        RectFace(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

    };

    class ColliderModule : public BaseModule {
    private:
        std::vector<std::shared_ptr<ColliderFace>> collider_faces_;
    public:
        static const std::size_t type;

        ColliderModule(std::vector<std::shared_ptr<ColliderFace>> collider_faces);
    };

    inline RayHitInfo RayCollisionDetection(ColliderModule *collider_m) {
        return {};
    }

}


#endif //HELLOWORLD_COLLIDER_MODULE_H
