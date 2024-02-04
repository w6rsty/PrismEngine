#include "editor_layer.hpp"


#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "scene/components.hpp"

namespace prism {

EditorLayer::EditorLayer()
: Layer("EditorLayer"),
  m_CameraController(1920.0f / 1080.0f) {

}

void EditorLayer::OnAttach() {
    m_CheckerboardTexture = Texture2D::Create("../../editor/assets/textures/Checkerboard.png");
	
    FrameBufferSpecification fbSpec;
    fbSpec.width = 1280;
    fbSpec.height = 720;
    m_FrameBuffer = FrameBuffer::Create(fbSpec);

    m_ActiveScene = CreateRef<Scene>();

    auto& reg = m_ActiveScene->Reg();
    const auto red = reg.create();
    reg.emplace<TransformComponent>(red, glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
    reg.emplace<SpriteRenderComponent>(red, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    const auto green = reg.create();
    reg.emplace<TransformComponent>(green, glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f)));
    reg.emplace<SpriteRenderComponent>(green, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
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

    if (m_ViewportFocused) {
        m_CameraController.OnUpdate(ts);
    }

    m_ActiveScene->OnUpdate(ts);

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

        Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_CheckerboardTexture);

        m_ActiveScene->OnRender();

        Renderer2D::EndScene();
        m_FrameBuffer->Unbind();
        RenderCommand::Clear();
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
    ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
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

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Instance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();



    if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
        // m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
    }

    uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
    
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

} // namespace prism