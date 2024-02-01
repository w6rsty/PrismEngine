#include "core/application.hpp"
#include "core/event/event.hpp"
#include "core/log_tag.hpp"
#include "core/logger.hpp"
#include "renderer/renderer.hpp"

#include "GLFW/glfw3.h"

#include <functional>

namespace prism {

#define BIND_APP_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name) {
    PRISM_PROFILE_FUNCTION();

    s_Instance = this;

    m_Window = Scope<Window>(Window::Create(WindowProps(name)));
    m_Window->SetEventCallback(BIND_APP_EVENT_FN(OnEvent));

    Renderer::Init();

#ifdef PRISM_IMGUI
    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
#endif
}

Application::~Application() {
    PRISM_PROFILE_FUNCTION();

    Renderer::Shutdown();
}

void Application::OnEvent(Event& event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_APP_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_APP_EVENT_FN(OnWindowResize));

    for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
        if(event.m_Handled) { 
            break; 
        }
        (*it)->OnEvent(event);
    }
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
    PRISM_PROFILE_FUNCTION();

    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& event) {
    PRISM_PROFILE_FUNCTION();

    if(event.GetWidth() == 0 || event.GetHeight() == 0) {
        LOG_TRACE(log_tag::Window, "Minimized");
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
    return false;
}

void Application::Run() {
    PRISM_PROFILE_FUNCTION();

    while(m_Running) {
        PRISM_PROFILE_SCOPE("RunLoop");

        float time = (float)glfwGetTime();
        Timestep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        if (!m_Minimized) {
            {
                PRISM_PROFILE_SCOPE("LayerStack OnUpdate");
                
                for (Layer* layer : m_LayerStack) {
                    layer->OnUpdate(timestep);
                }
            }
        }
#ifdef PRISM_IMGUI
        m_ImGuiLayer->Begin();
        {
            PRISM_PROFILE_SCOPE("LayerStack OnImGuiRender");
            for (Layer* layer : m_LayerStack) { 
                layer->OnImGuiRender();
            }
        }
        m_ImGuiLayer->End();
#endif
        m_Window->OnUpdate();
    }
}

void Application::Close() {
    m_Running = false;
}

void Application::PushLayer(Layer* layer) {
    PRISM_PROFILE_FUNCTION();

    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    PRISM_PROFILE_FUNCTION();

    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

#undef BIND_EVENT_FN
} // namespace prism