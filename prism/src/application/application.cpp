#include "application/application.hpp"
#include "event/event.hpp"
#include "renderer/renderer.hpp"

#include "GLFW/glfw3.h"

#include <functional>

namespace prism {

#define BIND_APP_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application() {
    s_Instance = this;

    m_Window = Scope<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_APP_EVENT_FN(OnEvent));

    Renderer::Init();

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

Application::~Application() {}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_APP_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_APP_EVENT_FN(OnWindowResize));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(event);
        if(event.m_Handled) { break; }
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& event) {
    if(event.GetWidth() == 0 || event.GetHeight() == 0) {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
    return false;
}

void Application::Run() {
    while(m_Running) {
        float time = (float)glfwGetTime();
        Timestep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        if (!m_Minimized) {
            for (Layer* layer : m_LayerStack) { layer->OnUpdate(timestep); }
        }

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack) { layer->OnImGuiRender(); }
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
    }
}

void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

#undef BIND_EVENT_FN
} // namespace prism