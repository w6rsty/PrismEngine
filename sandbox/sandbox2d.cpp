#include "sandbox2d.hpp"

#include "core/application.hpp"
#include "core/event/event.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "renderer/renderer2d.hpp"

Sandbox2D::Sandbox2D()
: Layer("Sandbox2D"),
  m_CameraController(1920.0f / 1080.0f) {

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

    prism::Renderer2D::ReSetStats();
    {
        PRISM_PROFILE_SCOPE("Renderer Prep");
        prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        prism::RenderCommand::Clear();
    }
    {
        PRISM_PROFILE_SCOPE("Renderer Draw");

        prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

        prism::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnEvent(prism::Event& event) {
    m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender() {
    PRISM_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = prism::Renderer2D::GetStats();
	ImGui::Text("FPS: %.2f", m_FPS);
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}