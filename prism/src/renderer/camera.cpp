#include "renderer/camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace prism {

Camera* Camera::CreateOrthographic(float left, float right, float bottom, float top) {
    return new OrthographicCamera(left, right, bottom, top);
}

Camera* Camera::CreatePerspective(float fov, float aspectRatio, float nearClip, float farClip) {
    return new PerspectiveCamera(fov, aspectRatio, nearClip, farClip);
}

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), 
  m_ViewMatrix(glm::mat4(1.0f)), 
  m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix),
  m_Position(0.0f), m_Rotation(0.0f) {
    
}

void OrthographicCamera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

///////////////////////////////////////////////////////////////////////////////

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
: m_ProjectionMatrix(glm::perspective(fov, aspectRatio, nearClip, farClip)), 
  m_ViewMatrix(glm::mat4(1.0f)), 
  m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix),
  m_Position(0.0f), m_Rotation(0.0f) {

}

void PerspectiveCamera::RecalculateViewMatrix() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

} // namespace prism