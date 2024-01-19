#include <iostream>
#include <string>

#include "prism.hpp"

#include "platform/OpenGL/opengl_shader.hpp"
#include "glm/ext/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "renderer/texture.hpp"

class AppLayer : public prism::Layer {
public:
    AppLayer() 
    : Layer("AppLayer"), 
      m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
      m_CameraPosition(0.0f, 0.0f, 0.0f) {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../../assets/fonts/JetBrainsMonoNerdFontMono-SemiBold.ttf", 24.0f);

        m_Color = glm::vec4(1.0, 1.0, 1.0, 1.0);
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

        std::string vertexSrc = R"(
            #version 460 core
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_TexCoord;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Model;

            out vec2 v_TexCoord;

            void main() {
                gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
                v_TexCoord = a_TexCoord;
            }
        )";
        std::string fragmentSrc = R"(
            #version 460 core
            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;

            uniform vec4 u_Color;
            uniform sampler2D u_Texture;
            
            void main() {
                color = texture(u_Texture, v_TexCoord) * u_Color;
            }
        )";

        m_Shader = prism::Shader::Create(vertexSrc, fragmentSrc);
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

        prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        prism::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        prism::Renderer::BeginScene(m_Camera);

        std::dynamic_pointer_cast<prism::OpenGLShader>(m_Shader)->Bind();
        std::dynamic_pointer_cast<prism::OpenGLShader>(m_Shader)->SetUniformInt("u_Texture", 0);
        std::dynamic_pointer_cast<prism::OpenGLShader>(m_Shader)->SetUniformFloat4("u_Color", m_Color);
        m_Texture->Bind();
    
        prism::Renderer::Submit(m_Shader, m_SquareVA);

        prism::Renderer::EndScene();
    }

    void OnImGuiRender() override {
        ImGui::Begin("Debug");
        ImGui::ColorEdit4("Square", glm::value_ptr(m_Color));
        ImGui::End();
    }


    void OnEvent(prism::Event& event) override {
        prism::EventDispatcher dispatcher(event);

    }
private:
    prism::Ref<prism::VertexArray> m_SquareVA;
    prism::Ref<prism::Shader> m_Shader;
    prism::Ref<prism::Texture> m_Texture;
    prism::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 1.0f;
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 180.0f;

    glm::vec4 m_Color;
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
