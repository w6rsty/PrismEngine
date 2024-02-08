#include "panel/scene_hierarchy_panel.hpp"

#include "scene/components.hpp"

#include "imgui.h"
#include "entt.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "scene/scene_camera.hpp"

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

    ImGui::End();

    ImGui::Begin("Properties");
    if (m_SelectedContext) {
        DrawEntity(m_SelectedContext);
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

    if (opened) {
        ImGui::TreePop();
    }
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

    if (entity.HasComponent<TransformComponent>()) {
        if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
            auto& transform = entity.GetComponent<TransformComponent>().Transform;

            ImGui::DragFloat3("Transform", glm::value_ptr(transform[3]), 0.25f);
            
            ImGui::TreePop();
        }
        ImGui::Separator();
    }

    if (entity.HasComponent<SpriteRenderComponent>()) {
        if (ImGui::TreeNodeEx((void*)typeid(SpriteRenderComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Color")) {
            auto& color = entity.GetComponent<SpriteRenderComponent>().Color;

            ImGui::ColorEdit4("Color", glm::value_ptr(color));
            
            ImGui::TreePop();
        }
        ImGui::Separator();
    }

    if (entity.HasComponent<CameraComponent>()) {
        if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
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

            ImGui::TreePop();
        }
    }
}

} // namespace prism