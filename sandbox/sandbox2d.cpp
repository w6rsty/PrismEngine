#include "sandbox2d.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "renderer/renderer2d.hpp"

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
    m_CameraController.OnUpdate(ts);

    prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    prism::RenderCommand::Clear();

    prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

    prism::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.5f, 0.7f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    prism::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f, }, { 1.0f, 1.0f }, m_Texture);

    prism::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(prism::Event& event) {
    m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender() {
    static bool show = true;
    ImGui::ShowDemoWindow(&show);
}