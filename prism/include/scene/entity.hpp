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
    T& AddComponent(Args&&... args) {
        PRISM_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
        T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        m_Scene->OnComponentAdded(typeid(T).name());
        return component;
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
    operator uint32_t() const { return (uint32_t)m_EntityHandle; }
    operator entt::entity() const { return m_EntityHandle; }
    bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }

    bool operator!=(const Entity& other) {
        return !operator==(other);
    }
private:
    entt::entity m_EntityHandle { entt::null };
    Scene* m_Scene = nullptr;
};

} // namespace prism