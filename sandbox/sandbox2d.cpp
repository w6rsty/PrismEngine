#include "sandbox2d.hpp"

#include "core/core.hpp"
#include "core/logger.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"

Sandbox2D::Sandbox2D()
: Layer("Sandbox2D"),
  m_CameraController(1920.0f / 1080.0f) {
    m_Texture = prism::Texture2D::Create("../../assets/textures/Checkerboard.png");
}

void Sandbox2D::OnAttach() {
    
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(prism::Timestep ts) {
    PRISM_PROFILE_FUNCTION();


    m_CameraController.OnUpdate(ts);


    {
        PRISM_PROFILE_SCOPE("Renderer Prep");
        prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        prism::RenderCommand::Clear();
    }

    {
        PRISM_PROFILE_SCOPE("Renderer Draw");
        prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

        prism::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 0.7f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        prism::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f, }, { 1.0f, 1.0f }, m_Texture);

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
        static bool show_demo = true;
        ImGui::ShowDemoWindow(&show_demo);
        ImGui::End();
    }
}