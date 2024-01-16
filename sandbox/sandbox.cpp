#include <iostream>

#include "glad/glad.h"
#include "prism.hpp"
#include "imgui.h"

class AppLayer : public prism::Layer {
public:
    AppLayer() : Layer("AppLayer") {}
    ~AppLayer() {}

    void OnUpdate() override {

    }

    void OnImGuiRender() override {
        ImGui::Begin("Renderer");
        ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
        ImGui::Text("Device: %s", glGetString(GL_RENDERER));
        ImGui::Text("Version: %s", glGetString(GL_VERSION));
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
