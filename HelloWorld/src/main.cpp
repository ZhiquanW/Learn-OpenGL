
#include <iostream>
#include "dawn_engine.h"
int main() {
    dawn::DawnEngine render_engine = dawn::DawnEngine(800, 800);
    render_engine.create_shader();
    render_engine.add_data();
    render_engine.launch();
    // delete render_engine;
}