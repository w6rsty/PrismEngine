#include <iostream>

#include "prism.hpp"

using namespace prism;

class Sandbox : public Application {
public:
    Sandbox() {}
    ~Sandbox() {}
};

Application* CreateApplication() {
    return new Sandbox();
}
