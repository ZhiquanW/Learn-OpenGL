#include <glad/glad.h>

#include "ZWEngine.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
// settings
const unsigned int SCR_WIDTH = 800;

const unsigned int SCR_HEIGHT = 600;

int main() {
    auto *tmp_app = new ZWEngine();
    if (!tmp_app->init_engine(SCR_WIDTH, SCR_HEIGHT)) {
        std::cout << "engine failed to initialize" << std::endl;
    } else {
        std::cout << "engine initialized successfully" << std::endl;
    }
    std::cout << "engine start running" << std::endl;
    tmp_app->run();
}