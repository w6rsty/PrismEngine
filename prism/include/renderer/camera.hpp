#pragma once

#include "glm/glm.hpp"

namespace prism {

class Camera {
public:
    virtual ~Camera() {};

    virtual const glm::vec3& GetPosition() const = 0;
    virtual void SetPosition(const glm::vec3& position) = 0;

    virtual float GetRotation() const = 0;
    virtual void SetRotation(float rotation) = 0;

    virtual const glm::mat4& GetProjectionMatrix() const = 0;
    virtual const glm::mat4& GetViewMatrix() const = 0;
    virtual const glm::mat4& GetViewProjectionMatrix() const = 0;

    static Camera* CreateOrthographic(float left, float right, float bottom, float top);
    static Camera* CreatePerspective(float fov, float aspectRatio, float nearClip, float farClip);
};

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    const glm::vec3& GetPosition() const override { return m_Position; }
    void SetPosition(const glm::vec3& position) override { m_Position = position; RecalculateViewMatrix(); }

    float GetRotation() const override { return m_Rotation; }
    void SetRotation(float rotation) override { m_Rotation = rotation; RecalculateViewMatrix(); }

    const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }
private:
    void RecalculateViewMatrix();
private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position;
    float m_Rotation;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

    const glm::vec3& GetPosition() const override { return m_Position; }
    void SetPosition(const glm::vec3& position) override{ m_Position = position; RecalculateViewMatrix(); }

    float GetRotation() const override { return m_Rotation; }
    void SetRotation(float rotation) override { m_Rotation = rotation; RecalculateViewMatrix(); }

    const glm::mat4& GetProjectionMatrix() const override{ return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }
private:
    void RecalculateViewMatrix();
private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position;
    float m_Rotation;
};
} // namespace prism