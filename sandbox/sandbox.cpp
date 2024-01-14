#include <iostream>

#include "prism.hpp"
#include "imgui/imgui_layer.hpp"

#include "glm/glm.hpp"

class AppLayer : public prism::Layer {
public:
    AppLayer() : Layer("AppLayer") {}
    ~AppLayer() {}

    void OnUpdate() override {
        if (prism::Input::IsMouseButtonPressed(PRISM_MOUSE_BUTTON_1)) {
            auto[x, y] = prism::Input::GetMousePosition();
            std::string msg = "Mouse position: " + std::to_string(x) + ", " + std::to_string(y);
            LOG_TRACE(prism::log_tag::Client, msg);

            glm::vec2 pos(x, y);
        }
    }

    void OnEvent(prism::Event& event) override {
        if (event.GetEventType() == prism::EventType::KeyPressed) {
            auto& e = static_cast<prism::KeyPressedEvent&>(event);
            LOG_TRACE(prism::log_tag::Client, (char)e.GetKeyCode());

        }
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
