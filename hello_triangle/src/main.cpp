#include <glad/glad.h>

#include "OpenGLApplication.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
// settings
const unsigned int SCR_WIDTH = 800;

const unsigned int SCR_HEIGHT = 600;

int main() {
    std::cout << "AAAAA" << std::endl;
    OpenGLApplication *tmp_app = new OpenGLApplication;
    if (!tmp_app->init_application(SCR_WIDTH, SCR_HEIGHT)) {
        std::cout << "Application failed to initialize" << std::endl;
    } else {
        std::cout << "Application initialized successfully" << std::endl;
    }
    tmp_app->run();
}