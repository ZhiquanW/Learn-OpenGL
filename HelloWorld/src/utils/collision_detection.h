//
// Created by zhiquan on 7/12/22.
//

#ifndef HELLOWORLD_COLLISION_DETECTION_H
#define HELLOWORLD_COLLISION_DETECTION_H

#include "common_includes.h"

class Ray {
private:
    glm::vec3 origin_ = glm::vec3(0.0f);
    glm::vec3 direction_ = glm::vec3(1.0f, 0.0f, 0.0f);
public:

    Ray() = default;

    Ray(glm::vec3 start_pos, glm::vec3 direction)
            : origin_(start_pos), direction_(glm::normalize(direction)) {
    }

    glm::vec3 ToDirection(float length) const {
        return this->origin_ + length * this->direction_;
    }

    glm::vec3 GetOrigin() const {
        return this->origin_;
    }

    void SetOrigin(glm::vec3 origin) {
        this->origin_ = origin;
    }


    glm::vec3 GetDirection() const {
        return this->direction_;
    }

    void SetDirection(glm::vec3 dir) {
        this->direction_ = dir;
    }
};

struct RayHitInfo {
    bool any_hit = false;
    float distance = 0.0f;
    glm::vec3 local_pos{};
    glm::vec3 global_pos{};
    glm::vec3 local_normal{};
    glm::vec3 global_normal{};
    dawn_engine::GameObject *game_object_ptr = nullptr;

    RayHitInfo() = default;

    RayHitInfo(bool any_hit, float distance, glm::vec3 local_pos, glm::vec3 local_normal) : any_hit(any_hit),
                                                                                            distance(distance),
                                                                                            local_pos(local_pos),
                                                                                            local_normal(local_normal) {
    }
};

struct ColliderBox {
    glm::vec3 origin = glm::vec3(0.0f);
    glm::vec3 dims = glm::vec3(1.0f); // x,y,z
    ColliderBox() {}

    ColliderBox(glm::vec3 origin, glm::vec3 dims) {
        this->origin = origin;
        this->dims = dims;
    }
};

class ColliderFace {
protected:
public:
    ColliderFace() = default;

    virtual RayHitInfo CollisionDetection(Ray ray) = 0;

    virtual glm::vec3 GetNormal() const = 0;
};

class RectFace : public ColliderFace {
protected:
    glm::vec3 origin_ = {};
    glm::vec3 edge_x_ = {};
    glm::vec3 edge_y_ = {};
public:

    RectFace(glm::vec3 origin, glm::vec3 v, glm::vec3 u)
            : origin_(origin), edge_x_(v), edge_y_(u) {
    }

    glm::vec3 GetOrigin() const {
        return this->origin_;
    }

    glm::vec3 GetEdgeX() const {
        return this->edge_x_;
    }

    glm::vec3 GetEdgeY() const {
        return this->edge_y_;
    }

    virtual glm::vec3 GetNormal() const {
        return glm::normalize(glm::cross(this->edge_x_, this->edge_y_));
    }

    virtual RayHitInfo CollisionDetection(Ray ray) {
        return {};
    }


};

// collision detection between a rat and a rectangle
// ray: origin, dir
// rectangle: origin, edge_x ,edge_y , normal = cross(edge_x, edge_y)

inline Ray TransformRay(glm::mat4 transform_mat, Ray ray) {
    return {glm::vec3(transform_mat * glm::vec4(ray.GetOrigin(), 1.0f)),
            glm::vec3(transform_mat * glm::vec4(ray.GetDirection(), 0.0f))};

}

inline RayHitInfo CollisionDetection(Ray ray, ColliderBox box, glm::mat4 scale_mat) {
    box.dims = glm::mat3(scale_mat) * box.dims;
    glm::vec3 box_min = box.origin - box.dims / 2.0f;
    glm::vec3 box_max = box.origin + box.dims / 2.0f;
    glm::vec3 min_xyz = (box_min - ray.GetOrigin()) / ray.GetDirection();
    glm::vec3 max_xyz = (box_max - ray.GetOrigin()) / ray.GetDirection();
    glm::vec3 dis_min = glm::min(min_xyz, max_xyz);
    glm::vec3 dis_max = glm::max(min_xyz, max_xyz);
    float t_near = glm::max(glm::max(dis_min.x, dis_min.y), dis_min.z);
    float t_far = glm::min(glm::min(dis_max.x, dis_max.y), dis_max.z);
    auto half_dims = box.dims / 2.0f;
    auto hit_pos = ray.ToDirection(t_near);
    glm::vec3 normals[] = {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)};
    auto normal = glm::vec3(0.0f);
    for (int i = 0; i < 3; ++i) {
        if (abs(hit_pos[i] - half_dims[i]) < 0.001f) {
            normal = normals[i];
            break;
        }
        if (abs(hit_pos[i] + half_dims[i]) < 0.001f) {
            normal = -normals[i];
            break;
        }
    }
    return {t_near < t_far, t_near, hit_pos, normal};
}

// uncompleted
inline RayHitInfo CollisionDetection(Ray ray, RectFace rect) {
    // check is ray is parallel to the plane (inside, below, above)
    const float EPSILON = 0.0001f;
    glm::vec3 n = rect.GetNormal();
    glm::vec3 d = ray.GetDirection();
    if (glm::abs(glm::dot(d, n)) < EPSILON) {
        return {};
    }
    // refer to bb's cs434
    // check hit distance on the plane of the rectangle
    glm::vec3 q = ray.GetOrigin() - rect.GetOrigin();
    float distance = -glm::dot(q, n) / glm::dot(d, n);
    float dot_x_y = glm::dot(rect.GetEdgeX(), rect.GetEdgeY());
    float u = dot_x_y / glm::length(rect.GetEdgeY());// ratio in edge x
    float v = dot_x_y / glm::length(rect.GetEdgeY());// ratio in edge y
    if (0 <= u && u <= 1)
        return RayHitInfo();
}


#endif //HELLOWORLD_COLLISION_DETECTION_H
