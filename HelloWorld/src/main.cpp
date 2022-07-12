
#include <iostream>
#include "hello_world_app.h"
#include <typeinfo>
#include "imgui.h"
#include "helloworld_ui_system.h"

void testCodes() {
    dawn_engine::GameObject cubeObject{true};
    // tmpObject.AddModule(dawn_engine::TransformModuleType);
    // std::cout << glm::to_string(tmpObject.GetModule<dawn_engine::TransformModule>().getRotationMat4());
    std::cout << glm::to_string(
            cubeObject.GetModule<dawn_engine::TransformModule>()->getScaleMat4()) << std::endl;
    // auto a = tmpObject.GetModule<dawn_engine::VisualShapeModule>();

    // std::cout << type(a) << std::endl;
}
// void run() {
//     dawn_engine::DawnEngine render_engine = dawn_engine::DawnEngine(1600, 1600, "hello world");
//     render_engine.InitShaderPrograms();
//     render_engine.loadTextures("../assets/test4.jpg", "../assets/test3.jpg");
//     render_engine.add_data();
//     render_engine.launch();
// }

void helloapp() {
    dawn_engine::DawnUISystem *b = new dawn_engine::DawnUISystem(430);
    helloworld::HelloWorldApp app(1600, 1600);
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