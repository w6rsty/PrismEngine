#include "sandbox2d.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"

Sandbox2D::Sandbox2D()
: Layer("Sandbox2D"),
  m_CameraController(1920.0f / 1080.0f) {
    m_SpriteSheet = prism::Texture2D::Create("../../assets/textures/tilemap_packed.png");
    m_SpriteSheet->SetWrap(prism::TextureWrap::ClampToEdge);

    m_TextureAxe = prism::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 0 }, { 16.0f, 16.0f }, { 1, 1 });
    m_TextureYellowTree = prism::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 3, 9 }, { 16.0f, 16.0f }, { 1, 2 });
    m_TextureGreenTree = prism::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 4, 9 }, { 16.0f, 16.0f }, { 1, 2 });
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
        prism::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.1f, 0.1f }, m_TextureAxe);
        prism::Renderer2D::DrawQuad({ 0.1f, 0.0f }, { 0.1f, 0.2f }, m_TextureYellowTree);
        prism::Renderer2D::DrawQuad({ 0.2f, 0.0f }, { 0.1f, 0.2f }, m_TextureGreenTree);
        prism::Renderer2D::DrawQuad({ 0.3f, 0.0f }, { 0.1f, 0.2f }, m_TextureGreenTree);

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

        auto stats = prism::Renderer2D::GetStats();

        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quad Count: %d", stats.quadCount);
        ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
        ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

        ImGui::End();
    }
}