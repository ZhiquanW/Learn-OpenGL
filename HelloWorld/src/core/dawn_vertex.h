//
// Created by zhiquan on 6/23/22.
//

#ifndef HELLOWORLD_DAWN_VERTEX_H
#define HELLOWORLD_DAWN_VERTEX_H

#include "common_includes.h"

namespace dawn_engine {
# define MAX_BONE_INFLUENCE 4

    struct DawnVertex {
        glm::vec3 position{};
        glm::vec3 normal{};
        glm::vec2 texCoords{};
        glm::vec3 tangent{};
        glm::vec3 biTangent{};
        int mBoneIDs[MAX_BONE_INFLUENCE] = {};
        float mWeights[MAX_BONE_INFLUENCE] = {};

        DawnVertex() = default;

        DawnVertex(glm::vec3 pos) : position(pos) {

        }
    };

}


#endif //HELLOWORLD_DAWN_VERTEX_H
