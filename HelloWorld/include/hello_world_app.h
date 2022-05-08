#pragma once

#include "dawn_engine.h"

namespace hello {
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
} // namespace hello