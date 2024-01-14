#pragma once

#include "prism.hpp"
#include <memory>

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
private:
    static Application* s_Instance;
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    LayerStack m_LayerStack;
};



} // namespace prism