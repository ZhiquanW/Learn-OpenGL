//
// Created by zhiquan on 7/14/22.
//

#ifndef HELLOWORLD_PREFABS_H
#define HELLOWORLD_PREFABS_H

#include "common_includes.h"

class BoxPrefab {
private:
    glm::vec3 size;
    std::vector<glm::vec3> joint_positions;
public:
    BoxPrefab();

};


#endif //HELLOWORLD_PREFABS_H
