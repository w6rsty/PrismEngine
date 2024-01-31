#pragma once

#include "renderer/camera.hpp"
#include "core/timestep.hpp"
#include "core/event/application_event.hpp"
#include "core/event/mouse_event.hpp"

#include "glm/glm.hpp"

namespace prism {

struct OrthographicCameraBounds {
    float Left, Right;
    float Bottom, Top;

    float GetWidth() { return Right - Left; }
    float GetHeight() { return Top - Bottom; }
};

class OrthographicCameraController {
public:
    OrthographicCameraController(float aspectRatio, bool rotation = false);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    OrthographicCamera& GetCamera() { return m_Camera; }
    const OrthographicCamera& GetCamera() const { return m_Camera; }

    void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }
    float GetZoomLevel() const { return m_ZoomLevel; }

    const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
private:
    void CalculateView();

    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);
private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    OrthographicCameraBounds m_Bounds;
    OrthographicCamera m_Camera;

    bool m_Rotation;

    glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
    float m_CameraRotation = 0.0f;
    float m_CameraTranslationSpeed = 1.0f;
    float m_CameraRotationSpeed = 180.0f;
};

} // namespace prism