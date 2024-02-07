#include "scene/scene_camera.hpp"

#include "glm/ext/matrix_clip_space.hpp"

namespace prism {

SceneCamera::SceneCamera() {

}

void SceneCamera::SetOrthoGraphic(float size, float near, float far) {
    m_OrthoGraphicSize = size;
    m_OrthoGraphicNear = near;
    m_OrthoGraphicFar = far;
    
    RecalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_AspectRatio = (float)width / (float)height;
    RecalculateProjection();
}

void SceneCamera::RecalculateProjection() {
    float orthoLeft     = -m_OrthoGraphicSize * m_AspectRatio * 0.5f;
    float orthoRight    =  m_OrthoGraphicSize * m_AspectRatio * 0.5f;
    float orthoBottom   = -m_OrthoGraphicSize  * 0.5f;
    float orthoTop      =  m_OrthoGraphicSize * 0.5f;

    m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, 
        m_OrthoGraphicNear, m_OrthoGraphicFar);
}

} // namespace prism