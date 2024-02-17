#pragma once

#include "glm/ext/matrix_transform.hpp"
#include "renderer/camera.hpp"
#include "scene/scene_camera.hpp"
#include "scene/scriptable_entity.hpp"

#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include <string>
#include <functional>

namespace prism {

struct TagComponent {
    std::string Tag;

    TagComponent(const std::string& tag)
    : Tag(tag) {}
};
struct TransformComponent {
    glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Scale = { 1.0f, 1.0f,1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation)
    : Translation(translation) {}

    glm::mat4 GetTransform() const {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
        return glm::translate(glm::mat4(1.0f), Translation)
            * rotation
            * glm::scale(glm::mat4(1.0f), Scale);
    }
};

struct SpriteRenderComponent {
    glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SpriteRenderComponent() = default;
    SpriteRenderComponent(const SpriteRenderComponent&) = default;
    SpriteRenderComponent(const glm::vec4& color)
    : Color(color) {}
};

struct CameraComponent {
    SceneCamera Camera;
    bool Primary = true;
    bool FixedAspectRatio = false;

    CameraComponent() {}
    CameraComponent(const CameraComponent&) = default; 
};

struct NativeScriptComponent {
    ScriptableEntity* Instance = nullptr;

    ScriptableEntity*(*InstantiateScript)();
    void(*DestroyScript)(NativeScriptComponent*);

    template <typename T>
    void Bind() {
        InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
        DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
    }
};

} // namespace prism