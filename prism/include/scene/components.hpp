#pragma once

#include "renderer/camera.hpp"
#include "scene/scene_camera.hpp"
#include "scene/scriptable_entity.hpp"

#include "glm/glm.hpp"

#include <string>
#include <functional>

namespace prism {

struct TagComponent {
    std::string name;

    TagComponent(const std::string& name)
    : name(name) {}
};
struct TransformComponent {
    glm::mat4 Transform;

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::mat4& transform)
    : Transform(transform) {}

    operator glm::mat4&() { return Transform; }
    operator const glm::mat4&() const { return Transform; }
};

struct SpriteRenderComponent {
    glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SpriteRenderComponent() = default;
    SpriteRenderComponent(const SpriteRenderComponent&) = default;
    SpriteRenderComponent(const glm::vec4& color)
    : Color(color) {}
};

struct CameraComponent {
    SceneCamera camera;
    bool primary = true;
    bool fixedAspectRatio = false;

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