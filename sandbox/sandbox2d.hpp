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
    prism::OrthographicCameraController m_CameraController;
    prism::Ref<prism::Texture> m_Texture;
};