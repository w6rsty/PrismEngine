#pragma once

#include "core/timestep.hpp"

#include "entt.hpp"

namespace prism {

class Scene {
public:
    Scene();
    ~Scene();

    void OnUpdate(Timestep ts);
    void OnRender();

    entt::registry& Reg() { return m_Registry; }
private:
    entt::registry m_Registry;
};

} // namespace prism