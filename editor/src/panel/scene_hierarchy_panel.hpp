#pragma once 

#include "core/core.hpp"
#include "scene/scene.hpp"
#include "scene/entity.hpp"

#include "imgui.h"
#include "imgui_internal.h"
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
    void DrawComponent(Entity entity, const std::string& name, bool removable, DrawComponentFn fn) {
        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap; 
        if (entity.HasComponent<T>()) {
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
            bool removeComponent = false;
            if (removable) {
                float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                ImGui::SameLine(ImGui::GetWindowWidth() - lineHeight - 3.0f);
                if (ImGui::Button("+", { lineHeight + 3.0f, lineHeight })) {
                    ImGui::OpenPopup("ComponentSettings");
                }
                if (ImGui::BeginPopup("ComponentSettings")) {
                    if (ImGui::MenuItem("Delete")) {
                        removeComponent = true;
                    }
                    ImGui::EndPopup();
                }
            }
            if (open) {
                fn(entity);
                ImGui::TreePop();
            }
            if (removeComponent) {
                entity.RemoveComponent<T>();
            }
        }
    }
private:
    Ref<Scene> m_Context;
    Entity m_SelectedContext;
};

} // namespace prism