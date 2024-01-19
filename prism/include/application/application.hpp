#pragma once

#include "core/core.hpp"
#include "core/timestep.hpp"
#include "window/window.hpp"
#include "imgui/imgui_layer.hpp"
#include "event/application_event.hpp"

namespace prism {

class Application {
public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(Event& event);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    inline static Application& Instance() { return *s_Instance; }
    inline Window& GetWindow() { return *m_Window; }
private:
    bool OnWindowClose(WindowCloseEvent& event);
    bool OnWindowResize(WindowResizeEvent& event);
private:
    Scope<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;
    LayerStack m_LayerStack;
    float m_LastFrameTime = 0.0f;
private:
    static Application* s_Instance;
};

} // namespace prism