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
        : Transform(transform) {
    }

    operator glm::mat4&() { return Transform; }
    operator const glm::mat4&() const { return Transform; }
};

struct SpriteRenderComponent {
    glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SpriteRenderComponent() = default;
    SpriteRenderComponent(const SpriteRenderComponent&) = default;
    SpriteRenderComponent(const glm::vec4& color)
        : Color(color) {
    }
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

    std::function<void()> InstantiateFunction;
    std::function<void()> DestroyInstanceFunction;

    std::function<void(ScriptableEntity*)> OnCreateFunction;
    std::function<void(ScriptableEntity*)> OnDestroyFunction;
    std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

    template <typename T>
    void Bind() {
        InstantiateFunction = [&]() { Instance = new T(); };
        DestroyInstanceFunction = [&]() { 
            delete (T*)Instance;
            Instance = nullptr;
        };

        OnCreateFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->OnCreate(); };
        OnDestroyFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->OnDestroy(); };
        OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { static_cast<T*>(instance)->OnUpdate(ts); };
    }
};

} // namespace prism