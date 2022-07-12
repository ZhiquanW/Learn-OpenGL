//
// Created by zhiquan on 7/7/22.
//

#include "collider_module.h"


namespace dawn_engine {

    Ray::Ray(glm::vec3 start_pos, glm::vec3 direction) : start_pos_(start_pos), direction_(glm::normalize(direction)) {

    }

    glm::vec3 Ray::ToDirection(float length) {
        return this->start_pos_ + length * this->direction_;
    }

    glm::vec3 Ray::GetStartPos() const {
        return this->start_pos_;
    }

    RectFace::RectFace(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
        this->faces_[0] = v0;
        this->faces_[1] = v1;
        this->faces_[2] = v2;
        this->faces_[3] = v3;

    }

    const std::size_t ColliderModule::type = BaseModule::getHashModuleName(quote(ColliderModule));

    ColliderModule::ColliderModule(std::vector<std::shared_ptr<ColliderFace>> collider_faces) : collider_faces_(collider_faces) {
    }

}