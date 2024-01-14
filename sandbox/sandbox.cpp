#include <iostream>

#include "prism.hpp"
#include "imgui/imgui_layer.hpp"

class AppLayer : public prism::Layer {
public:
    AppLayer() : Layer("AppLayer") {}
    ~AppLayer() {}

    void OnUpdate() override {
    }

    void OnEvent(prism::Event& event) override {

    }
};

class Sandbox : public prism::Application {
public:
    Sandbox() {
        PushLayer(new AppLayer());
        PushOverlay(new prism::ImGuiLayer());
    }
    ~Sandbox() {}
};

prism::Application* CreateApplication() {
    return new Sandbox();
}
