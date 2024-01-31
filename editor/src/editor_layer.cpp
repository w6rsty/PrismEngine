#include "editor_layer.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

namespace prism {

EditorLayer::EditorLayer()
: Layer("EditorLayer"),
  m_CameraController(1920.0f / 1080.0f) {

}

void EditorLayer::OnAttach() {
    m_CheckerboardTexture = Texture2D::Create("../../editor/assets/textures/Checkerboard.png");
	
    m_Particle.colorBegin = { 1.0, 137 / 255.0f, 0.0f, 1.0f};
	m_Particle.colorEnd = { 0.0f, 1.0f, 177 / 255.0f, 1.0f};
	m_Particle.sizeBegin = 1.0f;
	m_Particle.sizeEnd = 0.0f;
	m_Particle.sizeVariation = 0.3f;
	m_Particle.lifeTime = 1.2f;
	m_Particle.velocityVariation = { 1.5f, 1.2f, 0.0f };
	m_Particle.rotationBegin = 10.0f;
	m_ParticleCount = 4; 

    FrameBufferSpecification fbSpec;
    fbSpec.width = 1280;
    fbSpec.height = 720;
    m_FrameBuffer = FrameBuffer::Create(fbSpec);
}

void EditorLayer::OnDetach() {

}

void EditorLayer::OnUpdate(Timestep ts) {
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
		m_Particle.position = { pos.x, pos.y, 0.1f };
		for (int i = 0; i < m_ParticleCount; i++) {
			m_ParticleSystem.Emit(m_Particle);
		}
	}

    m_CameraController.OnUpdate(ts);
    m_ParticleSystem.OnUpdate(ts);
    Renderer2D::ReSetStats();

    {
        PRISM_PROFILE_SCOPE("Renderer Prep");
        m_FrameBuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();
    }
    {
        PRISM_PROFILE_SCOPE("Renderer Draw");

        Renderer2D::BeginScene(m_CameraController.GetCamera());

        Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture);
        m_ParticleSystem.OnRender();

        Renderer2D::EndScene();
        m_FrameBuffer->Unbind();
    }
}

void EditorLayer::OnEvent(Event& event) {
    m_CameraController.OnEvent(event);
}

void EditorLayer::OnImGuiRender() {
    PRISM_PROFILE_FUNCTION();

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
            if (ImGui::MenuItem("Exit")) Application::Instance().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Infos");

    auto stats = Renderer2D::GetStats();
    ImGui::Text("FPS: %.2f", m_FPS);
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Quads: %d", stats.quadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize) {
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        // m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
    }
    uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
    
    ImGui::End();
    ImGui::PopStyleVar();

    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Color begin", glm::value_ptr(m_Particle.colorBegin));
    ImGui::ColorEdit4("Color end", glm::value_ptr(m_Particle.colorEnd));
    ImGui::SliderFloat("Size begin", &m_Particle.sizeBegin, 0.0f, 1.0f);
    ImGui::SliderFloat("Size end", &m_Particle.sizeEnd, 0.0f, 1.0f);
    ImGui::SliderFloat("Size variation", &m_Particle.sizeVariation, 0.0f, 2.0f);
    ImGui::SliderFloat("Life time", &m_Particle.lifeTime, 0.0f, 5.0f);
    ImGui::SliderFloat2("Velocity variation", glm::value_ptr(m_Particle.velocityVariation), 0.0f, 5.0f);
    ImGui::SliderFloat("Rotation begin", &m_Particle.rotationBegin, 0.0f, 360.0f);
    ImGui::SliderInt("Particle count", (int*)&m_ParticleCount, 0, 20);

    ImGui::End();

    ImGui::End();
}

} // namespace prism