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

        m_Color = glm::vec4(1.0, 1.0, 1.0, 1.0);
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // 0
            0.5f, -0.5f, 0.0f, // 1
            0.5f,  0.5f, 0.0f, // 2
            -0.5f,  0.5f, 0.0f  // 3
        };
        prism::BufferLayout layout = {
            { prism::ShaderDataType::Float3, "a_Position" },
        };
        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_SquareVA.reset(prism::VertexArray::Create());
        
        std::shared_ptr<prism::VertexBuffer> squareVB;
        squareVB.reset(prism::VertexBuffer::Create(vertices, sizeof(vertices)));
        squareVB->SetLayout(layout);
        m_SquareVA->AddVertexBuffer(squareVB);
        std::shared_ptr<prism::IndexBuffer> squareIB;
        squareIB.reset(prism::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
            #version 460 core
            layout(location = 0) in vec3 a_Position;
            uniform mat4 u_ViewProjection;
            uniform mat4 u_Model;
            void main() {
                gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
            }
        )";
        std::string fragmentSrc = R"(
            #version 460 core
            layout(location = 0) out vec4 color;
            uniform vec4 u_Color;
            void main() {
                color = u_Color;
            }
        )";

        m_Shader.reset(prism::Shader::Create(vertexSrc, fragmentSrc));
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
        std::dynamic_pointer_cast<prism::OpenGLShader>(m_Shader)->SetUniformFloat4("u_Color", m_Color);
    

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                prism::Renderer::Submit(m_Shader, m_SquareVA, glm::translate(glm::mat4(1.0f), pos) * scale);
            }
        }
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
    std::shared_ptr<prism::VertexArray> m_SquareVA;
    std::shared_ptr<prism::Shader> m_Shader;
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
