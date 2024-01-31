#pragma once

#include "prism.hpp"

#include "particle_system.hpp"

namespace prism {

class EditorLayer : public Layer {
public:
    EditorLayer();
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Timestep ts) override;
    virtual void OnEvent(Event& event) override;
    virtual void OnImGuiRender() override;
private:
    float m_Time = 0.0f;
    int32_t m_FrameCount = 0;
    float m_FPS = 0.0f;

    OrthographicCameraController m_CameraController;
    Ref<FrameBuffer> m_FrameBuffer;
    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

    Ref<Texture> m_CheckerboardTexture;

    ParticleSystem m_ParticleSystem { 2000 };
    ParticleProps m_Particle;
    uint32_t m_ParticleCount = 0;
};

} // namespace prism