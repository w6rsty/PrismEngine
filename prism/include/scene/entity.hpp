#pragma once

#include "core./assert.hpp"
#include "scene/scene.hpp"

#include "entt.hpp"

namespace prism {

class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;

    template <typename T>
    bool HasComponent() {
        return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
    }

    template <typename T, typename... Args>
    Entity& AddComponent(Args&&... args) {
        PRISM_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
        m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        return *this;
    }

    template <typename T>
    T& GetComponent() {
        PRISM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template <typename T>
    void RemoveComponent() {
        PRISM_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    operator bool() const { return m_EntityHandle != entt::null; }
private:
    entt::entity m_EntityHandle { entt::null };
    Scene* m_Scene = nullptr;
};

} // namespace prism