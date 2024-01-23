#include "prism.hpp"

#include "platform/OpenGL/opengl_shader.hpp"
#include "glm/ext/matrix_transform.hpp" 

#include "imgui.h"

class AppLayer : public prism::Layer {
public:
    AppLayer() 
    : Layer("AppLayer"), 
      m_CameraController(1920.0f / 1080.0f) {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../../assets/fonts/JetBrainsMonoNerdFontMono-SemiBold.ttf", 24.0f);

        float vertices[] = {
            -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        };
        prism::BufferLayout layout = {
            { prism::ShaderDataType::Float3, "a_Position" },
            { prism::ShaderDataType::Float2, "a_TexCoord" },
        };
        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_SquareVA = prism::VertexArray::Create();
        
        prism::Ref<prism::VertexBuffer> squareVB;
        squareVB = prism::VertexBuffer::Create(vertices, sizeof(vertices));
        squareVB->SetLayout(layout);
        m_SquareVA->AddVertexBuffer(squareVB);
        prism::Ref<prism::IndexBuffer> squareIB = 
            prism::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_SquareVA->SetIndexBuffer(squareIB);

        m_Texture = prism::Texture2D::Create("../../assets/textures/prism_bright.png");

        m_ShaderLibrary.Load("texture", "../../assets/shaders/texture.glsl");
    }
    ~AppLayer() {}

    void OnUpdate(prism::Timestep ts) override {
        m_CameraController.OnUpdate(ts);

        prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        prism::RenderCommand::Clear();

        prism::Renderer::BeginScene(m_CameraController.GetCamera());

        auto textureShader = std::dynamic_pointer_cast<prism::OpenGLShader>(m_ShaderLibrary.Get("texture"));
        textureShader->Bind();
        textureShader->SetUniformInt("u_Texture", 0);
        m_Texture->Bind();

        prism::Renderer::Submit(textureShader, m_SquareVA);
        prism::Renderer::EndScene();
    }

    void OnImGuiRender() override {
    
    }

    void OnEvent(prism::Event& event) override {
        m_CameraController.OnEvent(event);
    }
private:
    prism::ShaderLibrary m_ShaderLibrary;
    prism::Ref<prism::VertexArray> m_SquareVA;
    prism::Ref<prism::Texture> m_Texture;
    prism::OrthographicCameraController m_CameraController;
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
