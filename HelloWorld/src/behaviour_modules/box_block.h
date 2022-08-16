//
// Created by zhiquan on 7/21/22.
//

#ifndef HELLOWORLD_BOX_BLOCK_H
#define HELLOWORLD_BOX_BLOCK_H

#include "behavior_module.h"

using namespace dawn_engine;

class AttachPoint {
private:
    glm::vec3 local_pos_{};
    glm::vec3 axis_{};
public:
    AttachPoint(glm::vec3 local_pos, glm::vec3 axis) : local_pos_(local_pos), axis_(axis) {

    }

    glm::vec3 GetLocalPos() const {
        return this->local_pos_;
    }

    glm::vec3 GetAxis() const {
        return axis_;
    }
};

class BoxBlock : public BehaviorModule {
public:
    glm::vec3 size{};
    std::vector<AttachPoint> joints{};

    explicit BoxBlock(glm::vec3 size) : BehaviorModule(__func__), size(size) {


    }

    void Awake() override {

    }

    void Start() override {

    }

    void Update() override;

    void UpdateModuleMonitor() override {
        if (ImGui::Button("save as prefab")) {
            this->Save2Json();
        }
    }

    BoxBlock() = default;


    void AddBlockJoint(const AttachPoint &joint) {
        this->joints.push_back(joint);
    }

    void AddAttachPoint(glm::vec3 local_pos, glm::vec3 axis) {
        this->joints.emplace_back(local_pos, axis);
    }

    void Save2Json();

};


#endif //HELLOWORLD_BOX_BLOCK_H
