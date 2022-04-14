
#include <iostream>
#include "hello_world_app.h"
#include <typeinfo>

void testCodes() {
    dawn::GameObject cubeObject{true};
    // tmpObject.addModule(dawn::TransformModuleType);
    // std::cout << glm::to_string(tmpObject.getModule<dawn::TransformModule>().getRotationMat4());
    std::cout << glm::to_string(cubeObject.getModule<dawn::TransformModule>()->getScaleMat4()) << std::endl;
    // auto a = tmpObject.getModule<dawn::VisualShapeModule>();

    // std::cout << type(a) << std::endl;
}
// void run() {
//     dawn::DawnEngine render_engine = dawn::DawnEngine(1600, 1600, "hello world");
//     render_engine.createShaderPrograms();
//     render_engine.loadTextures("../assets/test4.jpg", "../assets/test3.jpg");
//     render_engine.add_data();
//     render_engine.launch();
// }

void helloapp() {
    hello::HelloWorldApp app(1600, 1600);
    app.launch();
}

class Animal {
  public:
    virtual void eat() { std::cout << "I'm eating generic food.\n"; }
};

class Cat : public Animal {
  public:
    void eat() { std::cout << "I'm eating a rat.\n"; }
};
void print_eat(Animal *a) { a->eat(); }
void exp() {
    Animal *animal = new Animal;
    Cat *cat = new Cat;
}
int main() {
    // exp();
    helloapp();
    // delete render_engine;
}