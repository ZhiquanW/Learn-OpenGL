#pragma once

#include "dawn_engine.h"
namespace helloworld {
class HelloWorldApp : public dawn_engine::DawnEngine {
  private:
    void addDefaultLight();
    void addDefaultCube();

  public:
    HelloWorldApp(uint width, uint height);
    void awake();
    void start();
    void update();
};
} // namespace helloworld