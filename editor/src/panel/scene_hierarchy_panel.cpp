#include "panel/scene_hierarchy_panel.hpp"

#include "glm/trigonometric.hpp"
#include "scene/components.hpp"
#include "scene/scene.hpp"
#include "scene/scene_camera.hpp"

#include "entt.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "glm/gtc/type_ptr.hpp"

namespace prism {

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
    SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
    m_Context = context;
}

void SceneHierarchyPanel::OnImGuiRender() {
    ImGui::Begin("Scene Hierarchy");

    m_Context->m_Registry.view<TagComponent>().each([&](auto entityID, auto& tag) {
        DrawEntityNode(Entity {entityID, m_Context.get() });
    });

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
        m_SelectedContext = {};
    }

    ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup;
    if (ImGui::BeginPopupContextWindow(0, flags)) {
        if (ImGui::MenuItem("Create empty entity")) {
            m_Context->CreateEntity("Empty Entity");
        }
        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");
    if (m_SelectedContext) {
        DrawEntity(m_SelectedContext);

        ImGui::Separator();

        if (ImGui::Button("Add component")) {
            ImGui::OpenPopup("AddComponent");
        }

        if (ImGui::BeginPopup("AddComponent")) {
            if (ImGui::MenuItem("Camera")) {
                m_SelectedContext.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Sprite Renderer")) {
                m_SelectedContext.AddComponent<SpriteRenderComponent>();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
    auto& tc = entity.GetComponent<TagComponent>();

    ImGuiTreeNodeFlags flags = (entity == m_SelectedContext ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tc.tag.c_str());
    if (ImGui::IsItemClicked()) {
        m_SelectedContext = entity;
    }

    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete Entity")) {
            entityDeleted = true;
        }
        ImGui::EndPopup();
    }

    if (opened) {
        ImGui::TreePop();
    }

    if (entityDeleted) { 
        m_Context->DestroyEntity(entity);
        if (m_SelectedContext == entity) {
            m_SelectedContext = {};
        }
    }
}

static void DrawVec3Control(
    const std::string& label, 
    glm::vec3& value, 
    float resetValue = 0.0f,
    float speed = 0.1f,
    float min = 0.0f,
    float max = 1.0f,
    float columnWidth = 140.0f) 
{
    ImGui::PushID(label.c_str());
    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2, 2 });
    
    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, { 0.7f, 0.1f, 0.1f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.7f, 0.1f, 0.1f, 1.0f });
    if (ImGui::Button("X", buttonSize)) {
        value.x = resetValue;
    }
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##X", &value.x, speed, min, max, "%.1f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.5f, 0.0f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.1f, 0.8f, 0.1f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0f, 0.5f, 0.0f, 1.0f });
    if (ImGui::Button("Y", buttonSize)) {
        value.y = resetValue; 
    }
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &value.y, speed, min, max, "%.1f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, { 0.1f, 0.1f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.3f, 0.3f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1f, 0.1f, 0.8f, 1.0f });
    if (ImGui::Button("Z", buttonSize)) {
        value.z = resetValue;
    }
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &value.z, speed, min, max, "%.1f");
    ImGui::PopItemWidth();

    ImGui:;ImGui::PopStyleVar();

    ImGui::Columns(1);
    ImGui::PopID();
}

void SceneHierarchyPanel::DrawEntity(Entity entity) {
    if (entity.HasComponent<TagComponent>()) {
        auto& tag = entity.GetComponent<TagComponent>().tag;
        {
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }
        }
        ImGui::Separator();
    }

    DrawComponent<TransformComponent>(entity, "Transform", false, [](Entity entity) {
        auto& tc = entity.GetComponent<TransformComponent>();
        DrawVec3Control("Translation", tc.Translation, 0.0f, 0.1f, -1000.0f, 1000.0f);
        glm::vec3 rotation = glm::degrees(tc.Rotation);
        DrawVec3Control("Rotation", rotation, 0.0f, 1.0f, -1000.0f, 1000.0f);
        tc.Rotation = glm::radians(rotation);
        DrawVec3Control("Scale", tc.Scale, 1.0f, 0.1f, 0.0f, 100.0f);
    });

    DrawComponent<SpriteRenderComponent>(entity, "Color", true, [](Entity entity) {
        auto& color = entity.GetComponent<SpriteRenderComponent>().Color;
        ImGui::ColorEdit4("Color", glm::value_ptr(color));
    });

    DrawComponent<CameraComponent>(entity, "Camera", true, [](Entity entity) {
        auto& cameraComponent = entity.GetComponent<CameraComponent>();
        auto& camera = cameraComponent.camera;

        const char* projectionTypeString[] = { "Perspective", "Orthographic" };

        ImGui::Checkbox("Primary", &cameraComponent.primary);

        const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];
        if (ImGui::BeginCombo("Projetion", currentProjectionTypeString)) {
            for (int i = 0; i < 2; i++) {
                bool isSelected = currentProjectionTypeString == projectionTypeString[i];
                if (ImGui::Selectable(projectionTypeString[i], isSelected)) {
                    currentProjectionTypeString = projectionTypeString[i];
                    camera.SetProjectionType((SceneCamera::ProjectionType)i);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
            float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
            if (ImGui::DragFloat("Vertical FOV", &verticalFOV)) {
                camera.SetPerspectiveVerticalFOV(glm::radians(verticalFOV)); 
            }

            float perspectiveNearClip = camera.GetPerspectiveNearClip();
            if (ImGui::DragFloat("Near Clip", &perspectiveNearClip, 0.1f)) {
                camera.SetPerspectiveNearClip(perspectiveNearClip);
            }

            float perspectiveFarClip = camera.GetPerspectiveFarClip();
            if (ImGui::DragFloat("Far Clip", &perspectiveFarClip, 0.1f)) {
                camera.SetPerspectiveFarClip(perspectiveFarClip);
            }          
        }

        if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
            float orthoSize = camera.GetOrthographicSize();
            if (ImGui::DragFloat("Size", &orthoSize, 0.1f, 0.001f, 10.0f)) {
                camera.SetOrthographicSize(orthoSize); 
            }

            float orthoNearClip = camera.GetOrthographicNearClip();
            if (ImGui::DragFloat("Near Clip", &orthoNearClip, 0.1f)) {
                camera.SetOrthographicNearClip(orthoNearClip);
            }

            float orthoFarClip = camera.GetOrthographicFarClip();
            if (ImGui::DragFloat("Far Clip", &orthoFarClip, 0.1f)) {
                camera.SetOrthographicFarClip(orthoFarClip);
            }

            ImGui::Checkbox("Fixed aspect ratio", &cameraComponent.fixedAspectRatio);
        }
    });
}

} // namespace prism