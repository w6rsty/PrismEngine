#include "scene/scene_camera.hpp"

#include "glm/ext/matrix_clip_space.hpp"

namespace prism {

SceneCamera::SceneCamera() {

}

void SceneCamera::SetOrthographic(float size, float near, float far) {
    SetProjectionType(ProjectionType::Orthographic);
    m_OrthographicSize = size;
    m_OrthographicNearClip = near;
    m_OrthographicFarClip = far;
    RecalculateProjection();
}

void SceneCamera::SetPerspective(float verticalFOV, float near, float far) {
    SetProjectionType(ProjectionType::Perspective);
    m_PerspectiveFOV = verticalFOV;
    m_PerspectiveNearClip = near;
    m_PerspectiveFarClip = far;
    RecalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_AspectRatio = (float)width / (float)height;
    RecalculateProjection();
}

void SceneCamera::RecalculateProjection() {
    if (m_ProjectionType == ProjectionType::Orthographic) {
        float orthoLeft     = -m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoRight    =  m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthoBottom   = -m_OrthographicSize  * 0.5f;
        float orthoTop      =  m_OrthographicSize * 0.5f;

        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, 
            m_OrthographicNearClip, m_OrthographicFarClip);
    }

    if (m_ProjectionType == ProjectionType::Perspective) {
        m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, 
            m_PerspectiveNearClip, m_PerspectiveFarClip);
    }
}

} // namespace prism