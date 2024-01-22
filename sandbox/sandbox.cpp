#include <iostream>
#include <string>

#include "prism.hpp"

#include "platform/OpenGL/opengl_shader.hpp"
#include "glm/ext/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"


class AppLayer : public prism::Layer {
public:
    AppLayer() 
    : Layer("AppLayer"), 
      m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
      m_CameraPosition(0.0f, 0.0f, 0.0f) {
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

        m_Texture = prism::Texture2D::Create("../../assets/textures/Checkerboard.png");
        m_Logo = prism::Texture2D::Create("../../assets/textures/circle.png");

        m_ShaderLibrary.Load("texture", "../../assets/shaders/texture.glsl");
    }
    ~AppLayer() {}

    void OnUpdate(prism::Timestep ts) override {
        if (prism::Input::IsKeyPressed(PRISM_KEY_LEFT)) {
            m_CameraPosition.x -= m_CameraMoveSpeed * ts; 
        }
        else if (prism::Input::IsKeyPressed(PRISM_KEY_RIGHT)) {
            m_CameraPosition.x += m_CameraMoveSpeed * ts;
        }
        if (prism::Input::IsKeyPressed(PRISM_KEY_UP)) {
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        }
        else if (prism::Input::IsKeyPressed(PRISM_KEY_DOWN)) {
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;
        }

        if (prism::Input::IsKeyPressed(PRISM_KEY_A)) {
            m_CameraRotation -= m_CameraRotationSpeed * ts;
        }
        else if (prism::Input::IsKeyPressed(PRISM_KEY_D)) {
            m_CameraRotation += m_CameraRotationSpeed * ts;
        }

        if (prism::Input::IsKeyPressed(PRISM_KEY_ESCAPE)) {
            prism::Application::Instance().SetWindowState(false);
        }

        prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        prism::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        prism::Renderer::BeginScene(m_Camera);

        auto textureShader = std::dynamic_pointer_cast<prism::OpenGLShader>(m_ShaderLibrary.Get("texture"));
        textureShader->Bind();
        textureShader->SetUniformInt("u_Texture", 0);
        m_Texture->Bind();
    
        prism::Renderer::Submit(textureShader, m_SquareVA);

        m_Logo->Bind();
        prism::Renderer::Submit(textureShader, m_SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f)));

        prism::Renderer::EndScene();
    }

    void OnImGuiRender() override {
        // static bool show = true;
        // ImGui::ShowDemoWindow(&show);
    }


    void OnEvent(prism::Event& event) override {
        prism::EventDispatcher dispatcher(event);

    }
private:
    prism::ShaderLibrary m_ShaderLibrary;
    prism::Ref<prism::VertexArray> m_SquareVA;
    prism::Ref<prism::Texture> m_Texture, m_Logo;
    prism::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 1.0f;
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 180.0f;
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
