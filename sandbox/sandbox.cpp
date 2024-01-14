#include <iostream>

#include "prism.hpp"
#include "imgui.h"

class AppLayer : public prism::Layer {
public:
    AppLayer() : Layer("AppLayer") {}
    ~AppLayer() {}

    void OnUpdate() override {

    }

    void OnImGuiRender() override {
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
    }

    void OnEvent(prism::Event& event) override {

    }
};

class Sandbox : public prism::Application {
public:
    Sandbox() {
        PushLayer(new AppLayer());
    }
    ~Sandbox() {}
};

prism::Application* CreateApplication() {
    return new Sandbox();
}
