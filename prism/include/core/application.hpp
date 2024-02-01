#pragma once

#include "core/core.hpp"
#include "core/timestep.hpp"
#include "window/window.hpp"
#include "imgui/imgui_layer.hpp"
#include "event/application_event.hpp"

namespace prism {

class Application {
public:
    Application(const std::string& name = "Prism");
    virtual ~Application();

    void Run();
    void Close();
    void OnEvent(Event& event);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

    inline Window& GetWindow() { return *m_Window; }
    inline void SetWindowState(bool state) { m_Running = state; }
    inline static Application& Instance() { return *s_Instance; }
private:
    bool OnWindowClose(WindowCloseEvent& event);
    bool OnWindowResize(WindowResizeEvent& event);
private:
    Scope<Window> m_Window;
    bool m_Running = true;
    LayerStack m_LayerStack;
    float m_LastFrameTime = 0.0f;
    bool m_Minimized = false;
#ifdef PRISM_IMGUI
    ImGuiLayer* m_ImGuiLayer;
#endif
private:
    static Application* s_Instance;
};

} // namespace prism