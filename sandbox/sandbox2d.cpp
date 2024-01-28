#include "sandbox2d.hpp"

#include "core/core.hpp"
#include "core/logger.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"

Sandbox2D::Sandbox2D()
: Layer("Sandbox2D"),
  m_CameraController(1920.0f / 1080.0f) {
    m_Texture = prism::Texture2D::Create("../../assets/textures/prism.png");
    m_BackgroundTexture = prism::Texture2D::Create("../../assets/textures/Checkerboard.png");
}

void Sandbox2D::OnAttach() {
    
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(prism::Timestep ts) {
    PRISM_PROFILE_FUNCTION();

    m_Time += ts;
    m_FrameCount++;
    if (m_Time > 0.5f) {
        m_FPS = m_FrameCount / m_Time;
        m_FrameCount = 0;
        m_Time = 0.0f;
    }

    m_CameraController.OnUpdate(ts);


    {
        PRISM_PROFILE_SCOPE("Renderer Prep");
        prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        prism::RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;
        PRISM_PROFILE_SCOPE("Renderer Draw");
        prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

        prism::Renderer2D::DrawQuad({  0.2f,  0.2f }, { 0.6f, 0.6f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        prism::Renderer2D::DrawQuad({  0.4f,  0.4f }, { 0.6f, 0.6f }, { 0.2f, 0.8f, 0.3f, 1.0f });
        prism::Renderer2D::DrawQuad({  0.6f,  0.6f }, { 0.6f, 0.6f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        prism::Renderer2D::DrawRotatedQuad({ -1.0f,  0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_Texture);
        prism::Renderer2D::DrawQuad({  0.0f,  0.0f, -0.1f }, { 5.0f, 5.0f }, m_BackgroundTexture, 5.0f);

        prism::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnEvent(prism::Event& event) {
    m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender() {
    PRISM_PROFILE_FUNCTION();

    {
        PRISM_PROFILE_SCOPE("Sandbox2D::OnImGuiRender");
        ImGui::Begin("Settings");
        ImGui::Text("FPS: %d", (int)m_FPS);
        ImGui::End();

        static bool show_demo = true;
        ImGui::ShowDemoWindow(&show_demo);
    }
}