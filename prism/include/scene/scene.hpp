#pragma once

#include "core/timestep.hpp"

#include "entt.hpp"

namespace prism {

class Entity;

class Scene {
public:
    Scene();
    ~Scene();

    void OnUpdate(Timestep ts);
    void OnRender();

    Entity CreateEntity(const std::string& name);

    void OnViewportResize(uint32_t width, uint32_t height);

    friend class Entity;
private:
    entt::registry m_Registry;
    uint32_t m_ViewportWidth, m_ViewportHeight;
};

} // namespace prism