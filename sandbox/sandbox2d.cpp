#include "sandbox2d.hpp"

#include "core/core.hpp"
#include "core/logger.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "renderer/renderer2d.hpp"

Sandbox2D::Sandbox2D()
: Layer("Sandbox2D"),
  m_CameraController(1920.0f / 1080.0f) {
    m_Texture = prism::Texture2D::Create("../../assets/textures/prism.png");
    m_BackgroundTexture = prism::Texture2D::Create("../../assets/textures/Checkerboard.png");
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
        static float rotation = 0.0f;
        rotation += ts * 50.0f;
        PRISM_PROFILE_SCOPE("Renderer Draw");

        prism::Renderer2D::BeginScene(m_CameraController.GetCamera());
        prism::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_BackgroundTexture, 10.0f);

        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				prism::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}

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

        static bool show_demo = true;
        ImGui::ShowDemoWindow(&show_demo);
    }
}