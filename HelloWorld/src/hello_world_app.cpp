#include "hello_world_app.h"
#include <memory>
namespace hello {

HelloWorldApp::HelloWorldApp(uint width, uint height) : DawnEngine(width, height, "hello world") {
    // renderEngine = std::make_shared<dawn::DawnEngine>(width, height, "hello world");
}
} // namespace hello