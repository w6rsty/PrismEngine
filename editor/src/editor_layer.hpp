#pragma once

#include "prism.hpp"
#include "panel/scene_hierarchy_panel.hpp"

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

    bool m_ShowRenderInfo = false;

    OrthographicCameraController m_CameraController;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    Ref<FrameBuffer> m_FrameBuffer;
    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

    Ref<Scene> m_ActiveScene;
    SceneHierarchyPanel m_Panel;
    Ref<SceneSerializer> m_Serializer;
};

} // namespace prism