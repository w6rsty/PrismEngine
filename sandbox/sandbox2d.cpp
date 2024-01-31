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
	m_Particle.colorBegin = { 1.0, 137 / 255.0f, 0.0f, 1.0f};
	m_Particle.colorEnd = { 0.0f, 1.0f, 177 / 255.0f, 1.0f};
	m_Particle.sizeBegin = 1.0f;
	m_Particle.sizeEnd = 0.0f;
	m_Particle.sizeVariation = 0.3f;
	m_Particle.lifeTime = 1.2f;
	m_Particle.velocityVariation = { 1.5f, 1.2f, 0.0f };
	m_Particle.rotationBegin = 10.0f;
	m_ParticleCount = 4; 
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

	if (prism::Input::IsMouseButtonPressed(PRISM_MOUSE_BUTTON_1)) {
		auto [x, y] = prism::Input::GetMousePosition();
		x = (x / prism::Application::Instance().GetWindow().GetWidth()) * 2.0f - 1.0f;
		y = 1.0f - (y / prism::Application::Instance().GetWindow().GetHeight()) * 2.0f;
		auto pos = glm::inverse(m_CameraController.GetCamera().GetViewProjectionMatrix()) * glm::vec4(x, y, -1.0f, 1.0f);
		m_Particle.position = { pos.x, pos.y, 0.0f };
		for (int i = 0; i < m_ParticleCount; i++) {
			m_ParticleSystem.Emit(m_Particle);
		}
	}
    m_CameraController.OnUpdate(ts);
	m_ParticleSystem.OnUpdate(ts);
    prism::Renderer2D::ReSetStats();
    {
        PRISM_PROFILE_SCOPE("Renderer Prep");
        prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        prism::RenderCommand::Clear();
    }
    {
        PRISM_PROFILE_SCOPE("Renderer Draw");

        prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

		m_ParticleSystem.OnRender();

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