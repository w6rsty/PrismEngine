#include "application/application.hpp"
#include "event/event.hpp"
#include <functional>

namespace prism {

#define BIND_APP_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application() {
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_APP_EVENT_FN(OnEvent));
}

Application::~Application() {}

void Application::OnEvent(Event& event) {
    // LOG_TRACE(log_tag::App, event.ToString());
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_APP_EVENT_FN(OnWindowClose));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(event);
        if(event.m_Handled) {
            break;
        }
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    m_Running = false;
    return true;
}

void Application::Run() {
    while(m_Running) {
        glClear(GL_COLOR_BUFFER_BIT);
        for (Layer* layer : m_LayerStack) {
            layer->OnUpdate();
        }
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