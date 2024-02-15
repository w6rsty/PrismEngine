#include "editor_layer.hpp"

#include "panel/welcome_panel.hpp"

#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

#include <string>

namespace prism {

EditorLayer::EditorLayer()
: Layer("EditorLayer"),
  m_CameraController(1920.0f / 1080.0f) {

}

void EditorLayer::SetScene(const Ref<Scene>& scene) {
    m_ActiveScene = scene;
    m_Panel.SetContext(scene);
}

void EditorLayer::OnAttach() {	
    FrameBufferSpecification fbSpec;
    fbSpec.width = 1280 / 4;
    fbSpec.height = 720 / 4;
    m_FrameBuffer = FrameBuffer::Create(fbSpec);

    SetScene(CreateRef<Scene>());
}

void EditorLayer::OnDetach() {
    PRISM_PROFILE_FUNCTION();

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

    if (const auto& spec = m_FrameBuffer->GetSpecification(); 
        m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
        (spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
    {
        // m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }

    if (m_ViewportFocused) {
        m_CameraController.OnUpdate(ts);
    }

    Renderer2D::ReSetStats();

    m_FrameBuffer->Bind();
    RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    RenderCommand::Clear();

    m_ActiveScene->OnUpdate(ts);

    m_FrameBuffer->Unbind();
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
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 500.0f;
    
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {
                SetScene(CreateRef<Scene>());   
                m_SceneFilePath = "Default";
            }
            if (ImGui::MenuItem("Open")) {
                Ref<Scene> newScene = CreateRef<Scene>();
                SceneSerializer serializer(newScene);
                std::string filepath = FileDialog::OpenFile("Prism Scene (*.toml)\0*.toml\0");
                if (!filepath.empty()) {
                    SetScene(newScene);

                    serializer.Deserialize(filepath);
                    m_SceneFilePath = filepath;
                }
            }
            if (ImGui::MenuItem("Save", 0, false, !m_SceneFilePath.empty() && m_SceneFilePath != "Default")) {
                SceneSerializer serializer(m_ActiveScene);
                serializer.Serialize(m_SceneFilePath);
            }
            if (ImGui::MenuItem("Save As", 0, false, !m_SceneFilePath.empty())) {
                SceneSerializer serializer(m_ActiveScene);
                std::string filepath = FileDialog::SaveFile("Prism Scene (*.toml)\0*.toml\0");
                serializer.Serialize(filepath);
            }
            if (ImGui::MenuItem("Close Scene", 0, false, !m_SceneFilePath.empty())) {
                SetScene(CreateRef<Scene>());
                m_SceneFilePath.clear();
            }
            if (ImGui::MenuItem("Exit")) {
                Application::Instance().Close();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {   
            if (ImGui::MenuItem("Render Info")) m_ShowRenderInfo = !m_ShowRenderInfo;
            if (ImGui::MenuItem("Demo")) m_ShowDemo = !m_ShowDemo;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    if (m_ShowRenderInfo) {
        ImGui::Begin("Infos");
        auto stats = Renderer2D::GetStats();
        ImGui::Text("FPS: %.2f", m_FPS);
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        ImGui::End();
    }

    if (m_ShowDemo) {
        ImGui::ShowDemoWindow(&m_ShowDemo);
    }

    if (!m_SceneFilePath.empty()) {
        ImGuiWindowFlags viewport_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport", nullptr, viewport_flags);

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Instance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
        
        ImGui::End();
        ImGui::PopStyleVar();

        m_Panel.OnImGuiRender();
    } else {
        DrawWelcomePanel();
    }

    ImGui::End();
}

} // namespace prism