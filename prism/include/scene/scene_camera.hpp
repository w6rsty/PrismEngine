#pragma once

#include "renderer/camera.hpp"

namespace prism {

class SceneCamera : public Camera {
public:
    enum class ProjectionType { Perspective = 0, Orthographic = 1 };
public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float near, float far);
    void SetPerspective(float verticalFOV, float near, float far);

    void SetViewportSize(uint32_t width, uint32_t height);

    float GetOrthographicSize() const { return m_OrthographicSize; };
    void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); };
    float GetOrthographicNearClip() const { return m_OrthographicNearClip; }
    void SetOrthographicNearClip(float near){ m_OrthographicNearClip = near; RecalculateProjection(); }
    float GetOrthographicFarClip() const { return m_OrthographicFarClip; }
    void SetOrthographicFarClip(float far){ m_OrthographicFarClip = far; RecalculateProjection(); }

    float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; };
    void SetPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveFOV = verticalFOV; RecalculateProjection(); };
    float GetPerspectiveNearClip() const { return m_PerspectiveNearClip; }
    void SetPerspectiveNearClip(float near){ m_PerspectiveNearClip = near; RecalculateProjection(); }
    float GetPerspectiveFarClip() const { return m_PerspectiveFarClip; }
    void SetPerspectiveFarClip(float far){ m_PerspectiveFarClip = far; RecalculateProjection(); }

    ProjectionType GetProjectionType() const { return m_ProjectionType; }
    void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); } 
private:
    void RecalculateProjection();
private:
    ProjectionType m_ProjectionType = ProjectionType::Orthographic;

    float m_OrthographicSize = 10.0f;
    float m_OrthographicNearClip = -1.0f, m_OrthographicFarClip = 1.0f;

    float m_PerspectiveFOV = glm::radians(45.0f);
    float m_PerspectiveNearClip = 0.01f, m_PerspectiveFarClip = 1000.0f;

    float m_AspectRatio = 0.0f;
};

} // namespace prism