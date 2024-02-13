#pragma once

#include "core/timestep.hpp"

#include "entt.hpp"

namespace prism {

class Entity;
class SceneHierarchyPanel;
class SceneSerializer;

class Scene {
public:
    Scene();
    ~Scene();

    void OnUpdate(Timestep ts);

    Entity CreateEntity(const std::string& name);
    void DestroyEntity(Entity entity);

    void OnViewportResize(uint32_t width, uint32_t height);
private:
    void OnComponentAdded(const std::string& name);
private:
    entt::registry m_Registry;
    uint32_t m_ViewportWidth, m_ViewportHeight;

    friend class Entity;
    friend class SceneHierarchyPanel;
    friend class SceneSerializer;
};

} // namespace prism