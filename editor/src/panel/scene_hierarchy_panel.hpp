#pragma once 

#include "core/core.hpp"
#include "scene/scene.hpp"
#include "scene/entity.hpp"

namespace prism {

class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);

    void OnImGuiRender();

    void DrawEntityNode(Entity entity);
    void DrawEntity(Entity entity);
private:
    Ref<Scene> m_Context;
    Entity m_SelectedContext;
};

} // namespace prism