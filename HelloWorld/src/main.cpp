
#include <iostream>
#include "hello_world_app.h"
void testCodes() {
    dawn::GameObject cubeObject{true};
    // tmpObject.addModule(dawn::TransformModuleType);
    // std::cout << glm::to_string(tmpObject.getModule<dawn::TransformModule>().getRotationMat4());
    std::cout << glm::to_string(cubeObject.getModule<dawn::TransformModule>()->getScaleMat4()) << std::endl;
    // auto a = tmpObject.getModule<dawn::VisualShapeModule>();

    // std::cout << type(a) << std::endl;
}
void run() {
    dawn::DawnEngine render_engine = dawn::DawnEngine(1600, 1600, "hello world");
    render_engine.createShaderPrograms();
    render_engine.loadTextures("../assets/test4.jpg", "../assets/test3.jpg");
    render_engine.add_data();
    render_engine.launch();
}

void helloapp() {
    hello::HelloWorldApp app(1600, 1600);
    app.launch();
}
int main() {
    helloapp();
    // delete render_engine;
}