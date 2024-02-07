#include "editor_layer.hpp"

#include "scene/components.hpp"


#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include <string>
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

    m_SquareEntity = m_ActiveScene->CreateEntity("Square entity");
    m_SquareEntity.AddComponent<SpriteRenderComponent>(glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));

    class Player : public ScriptableEntity {
    public:
        void OnCreate() {
            std::cout << "Instantiating Player Script" << std::endl;
        }

        void OnDestroy() {

        }

        void OnUpdate(Timestep ts) {
            auto& transform = GetComponent<TransformComponent>().Transform;
            float speed = 5.0f;

            if (Input::IsKeyPressed(PRISM_KEY_A)) {
                transform[3][0] -= speed * ts;
            } else if (Input::IsKeyPressed(PRISM_KEY_D)) {
                transform[3][0] += speed * ts;
            }
            if (Input::IsKeyPressed(PRISM_KEY_W)) {
                transform[3][1] += speed * ts;
            } else if (Input::IsKeyPressed(PRISM_KEY_S)) {
                transform[3][1] -= speed * ts;
            }
        }
    };
    m_CameraEntity = m_ActiveScene->CreateEntity("Camera entity");
    m_CameraEntity.AddComponent<CameraComponent>();
    m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<Player>();

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

    if (const auto& spec = m_FrameBuffer->GetSpecification(); 
        m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
        (spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y)) {
        m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }

    if (m_ViewportFocused) {
        m_CameraController.OnUpdate(ts);
    }

    m_ActiveScene->OnUpdate(ts);

    Renderer2D::ReSetStats();

    {
        m_FrameBuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();

        m_ActiveScene->OnRender();

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

    ImGui::Separator();

    auto&  color = m_SquareEntity.GetComponent<SpriteRenderComponent>();
    ImGui::ColorEdit4("Square Color", glm::value_ptr(color.Color));

    ImGui::Separator();

    ImGui::SliderFloat3("Transform3", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]), -10.0f, 10.0f);

    {
        auto& camera = m_CameraEntity.GetComponent<CameraComponent>().camera;
        float orthoSize = camera.GetOrthoGraphicSize();
        if (ImGui::SliderFloat("Ortho Size", &orthoSize, 0.0f, 10.0f)) {
            camera.SetOrthoGraphicSize(orthoSize);
        }
    }

    ImGui::End();
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");

    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Instance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
    
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

} // namespace prism