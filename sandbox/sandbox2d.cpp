#include "sandbox2d.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"

Sandbox2D::Sandbox2D()
: Layer("Sandbox2D"),
  m_CameraController(1920.0f / 1080.0f) {
    m_Texture = prism::Texture2D::Create("../../assets/textures/prism.png");
    m_BackgroundTexture = prism::Texture2D::Create("../../assets/textures/Checkerboard.png");

    m_Particle.colorBegin = { 255 / 255.0f ,69/ 255.0f, 0.0f, 1.0f};
    m_Particle.colorEnd = { 0.2f, 0.4f, 0.2f, 1.0f};
    m_Particle.sizeBegin = 0.5f;
    m_Particle.sizeEnd = 0.0f;
    m_Particle.sizeVariation = 0.3f;
    m_Particle.lifeTime = 1.0f;
    m_Particle.velocityVariation = { 2.0f, 1.0f, 0.0f };
    m_Particle.rotationSpeed = 5.0f;

    m_ParticleCount = 5;
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

    if (prism::Input::IsMouseButtonPressed(PRISM_MOUSE_BUTTON_1)) {
        // get mouse position
        auto [x, y] = prism::Input::GetMousePosition();
        float width = prism::Application::Instance().GetWindow().GetWidth();
        float height = prism::Application::Instance().GetWindow().GetHeight();
        x = (x / width) * 2.0f - 1.0f;
        y = 1.0f - (y / height) * 2.0f;

        glm::vec4  pos = { x, y, 0.0f, 1.0f };
        pos = glm::inverse(m_CameraController.GetCamera().GetViewProjectionMatrix()) * pos;

        m_Particle.position = { pos.x, pos.y, 0.1f };
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

        m_ParticleSystem.OnRender();
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

        ImGui::ColorEdit4("Particle Color", glm::value_ptr(m_Particle.colorBegin));
        ImGui::ColorEdit4("Particle Color End", glm::value_ptr(m_Particle.colorEnd));
        ImGui::SliderFloat3("Velocity Variation", glm::value_ptr(m_Particle.velocityVariation), -5.0f, 5.0f);
        ImGui::SliderFloat("Particle Size", &m_Particle.sizeBegin, 0.0f, 2.0f);
        ImGui::SliderFloat("Rotation Speed", &m_Particle.rotationSpeed, 0.0f, 20.0f);
        ImGui::SliderFloat("Life Time", &m_Particle.lifeTime, 0.0f, 5.0f);
        ImGui::SliderInt("Particle Count", &m_ParticleCount, 0, 20);
        ImGui::End();
    }
}