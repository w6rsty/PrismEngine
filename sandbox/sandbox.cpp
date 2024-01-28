#include "prism.hpp"

#include "sandbox2d.hpp"

class Sandbox : public prism::Application {
public:
    Sandbox() {
        PushLayer(new Sandbox2D());
    }
    ~Sandbox() {}
};

prism::Application* CreateApplication() {
    return new Sandbox();
}
