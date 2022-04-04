#pragma once

#include "dawn_engine.h"

namespace hello {
class HelloWorldApp : public dawn::DawnEngine {
  private:
  public:
    HelloWorldApp(uint width, uint height);
};
} // namespace hello