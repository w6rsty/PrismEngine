#include "scene/scene_serializer.hpp"

#include "core/logger.hpp"
#include "scene/components.hpp"
#include "scene/entity.hpp"

#include "toml++/toml.hpp"
#include "entt.hpp"
#include "glm/glm.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

namespace prism {

SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
: m_Scene(scene) 
{

}

bool SceneSerializer::Serialize(const std::string& filepath) {
    LOG_INFO("Serializer", "Serializing scene to: ", filepath);

    auto tbl = toml::table {
        { "Scene", toml::table {
            { "ViewporSize", toml::table {
                { "Width", m_Scene->m_ViewportWidth },
                { "Height", m_Scene->m_ViewportHeight },
            }},
            { "Entities", toml::table {} },
        } },
    
    };

    auto view = m_Scene->m_Registry.view<TagComponent>();
    for (auto entity : view) {
        Entity e = { entity, m_Scene.get() };
        auto& tag = view.get<TagComponent>(entity);

        toml::table entityTable;
        entityTable.insert_or_assign("Tag", tag.Tag);
        if (e.HasComponent<TransformComponent>()) {
            auto& tc = e.GetComponent<TransformComponent>();
            entityTable.insert_or_assign("Transform", toml::table {
                { "Translation", toml::array { tc.Translation.x, tc.Translation.y, tc.Translation.z } },
                { "Rotation", toml::array { tc.Rotation.x, tc.Rotation.y, tc.Rotation.z } },
                { "Scale", toml::array { tc.Scale.x, tc.Scale.y, tc.Scale.z } },
            });
        }
        if (e.HasComponent<SpriteRenderComponent>()) {
            auto& spriteRenderer = e.GetComponent<SpriteRenderComponent>();
            entityTable.insert_or_assign("SpriteRenderer", toml::table {
                { "Color", toml::array { spriteRenderer.Color.r, spriteRenderer.Color.g, spriteRenderer.Color.b, spriteRenderer.Color.a } },
            });
        }
        if (e.HasComponent<CameraComponent>()) {
            auto& cameraComponent = e.GetComponent<CameraComponent>();
            entityTable.insert_or_assign("Camera", toml::table {
                { "Primary", cameraComponent.Primary },
                { "FixedAspectRatio", cameraComponent.FixedAspectRatio },
                { "Camera", toml::table {
                        { "ProjectionType", (int)cameraComponent.Camera.GetProjectionType() },
                        { "PerspectiveFOV", cameraComponent.Camera.GetPerspectiveVerticalFOV() },
                        { "PerspectiveNear", cameraComponent.Camera.GetPerspectiveNearClip() },
                        { "PerspectiveFar", cameraComponent.Camera.GetPerspectiveFarClip() },
                        { "OrthographicSize", cameraComponent.Camera.GetOrthographicSize() },
                        { "OrthographicNear", cameraComponent.Camera.GetOrthographicNearClip() },
                        { "OrthographicFar", cameraComponent.Camera.GetOrthographicFarClip() },
                    }
                }
            });
        }

        std::string id = std::to_string(uint32_t(entity));
        tbl["Scene"]["Entities"].as_table()->insert_or_assign(id, entityTable);
    }

    std::ofstream outputFile(filepath);
    outputFile << tbl;
    outputFile.close();

    return true;
}

bool SceneSerializer::Deserialize(const std::string& filepath) {
    LOG_INFO("Serializer", "Deserializing scene from: ", filepath);

    std::ifstream input(filepath);

    toml::parse_result res = toml::parse(input);    

    if (!res) {
        LOG_ERROR("Serializer", "Failed to parse scene file");
        return false;
    }

    m_Scene->m_Registry.clear();
    auto entities = res["Scene"]["Entities"].as_table();
    for (auto& [id, entity] : *entities) {
        toml::table& entityTable = *entity.as_table();
        std::string tag = entityTable["Tag"].as_string()->get();

        Entity deserializedEntity = m_Scene->CreateEntity(tag);

        if (entityTable.contains("Transform")) {
            toml::table& transform = *entityTable["Transform"].as_table();
            auto translation = transform["Translation"].as_array();
            auto rotation = transform["Rotation"].as_array();
            auto scale = transform["Scale"].as_array();

            auto& tc = deserializedEntity.GetComponent<TransformComponent>();
            tc.Translation = {
                translation->get(0)->as_floating_point()->get(),
                translation->get(1)->as_floating_point()->get(),
                translation->get(2)->as_floating_point()->get(),
            };
            tc.Rotation = {
                rotation->get(0)->as_floating_point()->get(),
                rotation->get(1)->as_floating_point()->get(),
                rotation->get(2)->as_floating_point()->get(),
            };
            tc.Scale = {
                scale->get(0)->as_floating_point()->get(),
                scale->get(1)->as_floating_point()->get(),
                scale->get(2)->as_floating_point()->get(),
            };
        }

        if (entityTable.contains("SpriteRenderer")) {
            toml::table& spriteRenderer = *entityTable["SpriteRenderer"].as_table();
            auto color = spriteRenderer["Color"].as_array();

            auto& src = deserializedEntity.AddComponent<SpriteRenderComponent>();
            src.Color = {
                color->get(0)->as_floating_point()->get(),
                color->get(1)->as_floating_point()->get(),
                color->get(2)->as_floating_point()->get(),
                color->get(3)->as_floating_point()->get(),
            };
        }


        if (entityTable.contains("Camera")) {
            toml::table& camera = *entityTable["Camera"].as_table();
            auto& cc = deserializedEntity.AddComponent<CameraComponent>();
            cc.Primary = camera["Primary"].as_boolean()->get();
            cc.FixedAspectRatio = camera["FixedAspectRatio"].as_boolean()->get();

            auto& cameraProps = *camera["Camera"].as_table();
            cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as_integer()->get());
            cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as_floating_point()->get());
            cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as_floating_point()->get());
            cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as_floating_point()->get());
            cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as_floating_point()->get());
            cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as_floating_point()->get());
            cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as_floating_point()->get());
        }

    }

    return true;
}

} // namespace prism