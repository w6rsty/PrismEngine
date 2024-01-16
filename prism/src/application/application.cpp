#include "application/application.hpp"
#include "event/event.hpp"
#include <functional>
#include "glad/glad.h"

namespace prism {

#define BIND_APP_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

static GLenum  ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:  return GL_FLOAT;
        case ShaderDataType::Float2: return GL_FLOAT;
        case ShaderDataType::Float3: return GL_FLOAT;
        case ShaderDataType::Float4: return GL_FLOAT;
        case ShaderDataType::Mat3:   return GL_FLOAT;
        case ShaderDataType::Mat4:   return GL_FLOAT;
        case ShaderDataType::Int:    return GL_INT;
        case ShaderDataType::Int2:   return GL_INT;
        case ShaderDataType::Int3:   return GL_INT;
        case ShaderDataType::Int4:   return GL_INT;
        case ShaderDataType::Bool:   return GL_BOOL;
        case ShaderDataType::None: {
            PRISM_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
        }
    }
}

Application* Application::s_Instance = nullptr;

Application::Application() {
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_APP_EVENT_FN(OnEvent));
    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };

    uint32_t indices[3] = { 0, 1, 2 };

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);
    m_VertexBuffer = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(vertices, sizeof(vertices)));
    m_IndexBuffer = std::unique_ptr<IndexBuffer>(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    
    BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color"}
    };

    m_VertexBuffer->SetLayout(layout);

    uint32_t index = 0;
    for (const auto& element : m_VertexBuffer->GetLayout()) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 
                              element.GetComponentCount(),
                              ShaderDataTypeToOpenGLBaseType(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE, 
                              layout.GetStride(), 
                              reinterpret_cast<const void*>(element.offset));
        index++;
    }

    std::string vertexSrc = R"(
        #version 460 core
        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;
        out vec4 v_Color;
        void main() {
            gl_Position = vec4(a_Position, 1.0);
            v_Color = a_Color;
        }
    )";
    std::string fragmentSrc = R"(
        #version 460 core
        layout(location = 0) out vec4 color;
        in vec4 v_Color;
        void main() {
            color = v_Color;
        }
    )";

    m_Shader = std::unique_ptr<Shader>(new Shader(vertexSrc, fragmentSrc));
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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Shader->Bind();
        glBindVertexArray(m_VertexArray);
        glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

        for (Layer* layer : m_LayerStack) {
            layer->OnUpdate();
        }

        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack) {
            layer->OnImGuiRender();
        }
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