#pragma once 

#include "core/core.hpp"
#include "scene/scene.hpp"
#include "scene/entity.hpp"

#include "imgui.h"
#include "glm/glm.hpp"

namespace prism {

class SceneHierarchyPanel {
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);

    void OnImGuiRender();

    void DrawEntityNode(Entity entity);
    void DrawEntity(Entity entity);

    using DrawComponentFn = void(*)(Entity);

    template <typename T>
    void DrawComponent(Entity entity, const std::string& name, DrawComponentFn fn) {
        if (entity.HasComponent<T>()) {
            if (ImGui::TreeNodeEx(
                    (void*)typeid(T).hash_code(),
                    ImGuiTreeNodeFlags_DefaultOpen, 
                    "%s", name.c_str())) {
                fn(entity);
                ImGui::TreePop();
            }
        }
    }
private:
    Ref<Scene> m_Context;
    Entity m_SelectedContext;
};

} // namespace prism