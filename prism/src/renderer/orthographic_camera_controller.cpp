#include "renderer/orthographic_camera_controller.hpp"
#include "core/logger.hpp"
#include "core/event/event.hpp"
#include "core/window/keycodes.hpp"
#include "core/window/input.hpp"
#include "prism.hpp"

#include "glad/glad.h"

namespace prism {

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
: m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
  m_AspectRatio(aspectRatio),
  m_Rotation(rotation) {

}

void OrthographicCameraController::OnUpdate(Timestep ts) {
    PRISM_PROFILE_FUNCTION();

    if (Input::IsKeyPressed(PRISM_KEY_A)) {
        m_CameraPosition.x -= m_CameraTranslationSpeed * ts; 
    }
    else if (Input::IsKeyPressed(PRISM_KEY_D)) {
        m_CameraPosition.x += m_CameraTranslationSpeed * ts;
    }
    if (Input::IsKeyPressed(PRISM_KEY_W)) {
        m_CameraPosition.y += m_CameraTranslationSpeed * ts;
    }
    else if (Input::IsKeyPressed(PRISM_KEY_S)) {
        m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
    }

    if (m_Rotation) {
        if (Input::IsKeyPressed(PRISM_KEY_Q)) {
            m_CameraRotation -= m_CameraRotationSpeed * ts;
        }
        else if (Input::IsKeyPressed(PRISM_KEY_E)) {
            m_CameraRotation += m_CameraRotationSpeed * ts;
        }
        m_Camera.SetRotation(m_CameraRotation);
    }

    m_Camera.SetPosition(m_CameraPosition);

    m_CameraTranslationSpeed = m_ZoomLevel;
}

void OrthographicCameraController::OnEvent(Event& e)  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(PRISM_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(PRISM_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
};

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
    m_ZoomLevel -= e.GetYOffset() * 0.1f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return false;
};

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    glViewport(0, 0, e.GetWidth(), e.GetHeight());
    return false;
};

} // namespace prism