#pragma once

#include "prism.hpp"

struct ProfileResult {
    const char* name;
    float time;
};

class Sandbox2D : public prism::Layer {
public:
    Sandbox2D();
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(prism::Timestep ts) override;
    virtual void OnEvent(prism::Event& event) override;
    virtual void OnImGuiRender() override;
private:
    float m_Time = 0.0f;
    int32_t m_FrameCount = 0;
    float m_FPS = 0.0f;

    prism::OrthographicCameraController m_CameraController;
    prism::Ref<prism::Texture> m_Texture;
    prism::Ref<prism::Texture> m_BackgroundTexture;
};