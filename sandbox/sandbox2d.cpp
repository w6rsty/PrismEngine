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

    prism::FrameBufferSpecification fbSpec;
    fbSpec.width = 160;
    fbSpec.height = 90;
    m_FrameBuffer = prism::FrameBuffer::Create(fbSpec);
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
		auto width = prism::Application::Instance().GetWindow().GetWidth();
		auto height = prism::Application::Instance().GetWindow().GetHeight();
		x = (x / width) * 2.0f - 1.0f;
		y = 1.0f - (y / height) * 2.0f;
		auto inverse = glm::inverse(m_CameraController.GetCamera().GetProjectionMatrix());
		auto pos = inverse * glm::vec4(x, y, -1.0f, 1.0f);
		m_Particle.position = { pos.x, pos.y, 0.0f };
		for (int i = 0; i < m_ParticleCount; i++)
			m_ParticleSystem.Emit(m_Particle);

	}
    m_CameraController.OnUpdate(ts);
	m_ParticleSystem.OnUpdate(ts);
    prism::Renderer2D::ReSetStats();

    {
        PRISM_PROFILE_SCOPE("Renderer Prep");
        m_FrameBuffer->Bind();
        prism::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        prism::RenderCommand::Clear();
    }
    {
        PRISM_PROFILE_SCOPE("Renderer Draw");

        prism::Renderer2D::BeginScene(m_CameraController.GetCamera());

		m_ParticleSystem.OnRender();

        prism::Renderer2D::EndScene();
        m_FrameBuffer->Unbind();
    }
}

void Sandbox2D::OnEvent(prism::Event& event) {
    m_CameraController.OnEvent(event);
}

void Sandbox2D::OnImGuiRender() {
    PRISM_PROFILE_FUNCTION();

	static bool dockingEnabled = true;
	if (dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) prism::Application::Instance().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = prism::Renderer2D::GetStats();
		ImGui::Text("FPS: %.2f", m_FPS);
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());


		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, { 1280, 720 }, { 0, 1 }, { 1, 0 });
		ImGui::End();

		ImGui::End();
	}
}