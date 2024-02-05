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

    Entity CreateEntity();

    friend class Entity;
private:
    entt::registry m_Registry;
};

} // namespace prism