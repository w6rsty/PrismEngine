#include "prism.hpp"

#include "sandbox2d.hpp"

class Sandbox : public prism::Application {
public:
    Sandbox() {
        PushLayer(new Sandbox2D());
        GetWindow().SetVSync(false);
    }
    ~Sandbox() {}
};

prism::Application* CreateApplication() {
    return new Sandbox();
}
