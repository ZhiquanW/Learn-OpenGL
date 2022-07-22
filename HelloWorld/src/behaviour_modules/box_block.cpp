//
// Created by zhiquan on 7/22/22.
//

#include "box_block.h"
#include "core/dawn_engine.h"

void BoxBlock::Save2Json() {
    nlohmann::json j;
    j["geom"] = "box";
    j["size"] = {this->size.x, this->size.y, this->size.z};
    j["attach_points"] = {};
    for (const auto &joint: this->joints) {
        j["attach_points"].push_back({joint.GetLocalPos().x, joint.GetLocalPos().y, joint.GetLocalPos().z});
    }
    std::ofstream file(fmt::format("../prefabs/{}.json", this->owner_ptr_->GetName()));
    file << j;
    DawnEngine::instance->AddPrefabName(this->owner_ptr_->GetName());

}

void BoxBlock::Update() {
    this->size = this->owner_ptr_->GetModule<TransformModule>()->GetScale();
}
