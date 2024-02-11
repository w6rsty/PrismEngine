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


    template <typename T, typename UIFn>
    static void DrawComponent(Entity entity, const std::string& name, bool removable, UIFn function) {
        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen 
            | ImGuiTreeNodeFlags_AllowItemOverlap
            | ImGuiTreeNodeFlags_Framed
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_FramePadding; 
        if (entity.HasComponent<T>()) {
            auto& component = entity.GetComponent<T>();
            bool removeComponent = false;
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
            ImGui::PopStyleVar();

            if (removable) {
                ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
                if (ImGui::Button("+", ImVec2(lineHeight, lineHeight))) {
                    ImGui::OpenPopup("ComponentSettings");
                }

                if (ImGui::BeginPopup("ComponentSettings")) {
                    if (ImGui::MenuItem("Delete")) { removeComponent = true; }
                    ImGui::EndPopup();
                }
            }
            if (open) {
                // Draw the component
                function(component);
                ImGui::TreePop();
            }
            if (removeComponent) { entity.RemoveComponent<T>(); }
        }
    }
private:
    Ref<Scene> m_Context;
    Entity m_SelectedContext;
};

} // namespace prism