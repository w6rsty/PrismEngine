#include "scene/scene_serializer.hpp"

#include "core/logger.hpp"
#include "scene/components.hpp"
#include "scene/entity.hpp"

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

    toml::table tbl;

    // Scene Info
    SerializeScene(tbl);

    // Entities
    toml::array entities;
    m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID) {
        Entity entity = { entityID, m_Scene.get() };

        if (!entity) return;

        SerializeEntity(entities, entity); 
    });
    tbl.insert_or_assign("Entities", entities);


    std::ofstream outputFile(filepath);
    outputFile << tbl;
    outputFile.close();

    return true;
}

bool SceneSerializer::Deserialize(const std::string& filepath) {
    LOG_INFO("Serializer", "Deserializing scene from: ", filepath);

    std::ifstream input(filepath);

    toml::parse_result result;
    try {
        result = toml::parse(input);
    } catch (const toml::parse_error& err) {
        LOG_ERROR("Serializer", "Failed to parse scene file: ", err.description());
        return false;
    }

    toml::table tbl = result.table();

    DeserializeScene(tbl);

    auto& entities = *tbl["Entities"].as_array();
    DeserializeEntity(entities);

    return true;
}


void SceneSerializer::SerializeScene(toml::table& sceneTable) {
    sceneTable.insert_or_assign("Scene", m_Scene->m_Name);
    sceneTable.insert_or_assign("ViewportSize", toml::table {
        { "Width", m_Scene->m_ViewportWidth },
        { "Height", m_Scene->m_ViewportHeight },
    });
}

void SceneSerializer::DeserializeScene(const toml::table& sceneTable) {
    auto viewportSize = sceneTable["ViewportSize"].as_table();
    m_Scene->m_Name = sceneTable["Scene"].as_string()->get();
    m_Scene->m_ViewportWidth = viewportSize->get("Width")->as_integer()->get();
    m_Scene->m_ViewportHeight = viewportSize->get("Height")->as_integer()->get();
}

void SceneSerializer::SerializeEntity(toml::array& array, Entity entity) {
    toml::table entityTable;

    auto& tag = entity.GetComponent<TagComponent>().Tag;
    entityTable.insert_or_assign("Tag", tag);

    if (entity.HasComponent<TransformComponent>()) {
        auto& tc = entity.GetComponent<TransformComponent>();

        entityTable.insert_or_assign("Transform", toml::table {
            { "Translation",    toml::array { tc.Translation.x, tc.Translation.y,   tc.Translation.z } },
            { "Rotation",       toml::array { tc.Rotation.x,    tc.Rotation.y,      tc.Rotation.z } },
            { "Scale",          toml::array { tc.Scale.x,       tc.Scale.y,         tc.Scale.z } },
        });
    }

    if (entity.HasComponent<SpriteRenderComponent>()) {
        auto& src = entity.GetComponent<SpriteRenderComponent>();

        entityTable.insert_or_assign("SpriteRender", toml::table {
            { "Color",          toml::array { src.Color.r, src.Color.g, src.Color.b, src.Color.a } },
        });
    }

    if (entity.HasComponent<CameraComponent>()) {
        auto& cc = entity.GetComponent<CameraComponent>();

        entityTable.insert_or_assign("Camera", toml::table {
            { "Primary", cc.Primary },
            { "FixedAspectRatio", cc.FixedAspectRatio },
            { "Camera", toml::table {
                { "ProjectionType",     (int)cc.Camera.GetProjectionType() },
                { "PerspectiveFOV",     cc.Camera.GetPerspectiveVerticalFOV() },
                { "PerspectiveNear",    cc.Camera.GetPerspectiveNearClip() },
                { "PerspectiveFar",     cc.Camera.GetPerspectiveFarClip() },
                { "OrthographicSize",   cc.Camera.GetOrthographicSize() },
                { "OrthographicNear",   cc.Camera.GetOrthographicNearClip() },
                { "OrthographicFar",    cc.Camera.GetOrthographicFarClip() },
            }},
        });
    }

    array.push_back(entityTable);
}

void SceneSerializer::DeserializeEntity(const toml::array& array) {
    for (auto& entity : array) {
        auto& entityTable = *entity.as_table();

        std::string tag = entityTable["Tag"].as_string()->get();
        Entity deserializedEntity = m_Scene->CreateEntity(tag);

        if (entityTable.contains("Transform")) {
            auto& transform = *entityTable["Transform"].as_table();
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

        if (entityTable.contains("SpriteRender")) {
            auto& sc = *entityTable["SpriteRender"].as_table();
            auto color = sc["Color"].as_array();

            auto& src = deserializedEntity.AddComponent<SpriteRenderComponent>();
            src.Color = {
                color->get(0)->as_floating_point()->get(),
                color->get(1)->as_floating_point()->get(),
                color->get(2)->as_floating_point()->get(),
                color->get(3)->as_floating_point()->get(),
            };
        }

        if (entityTable.contains("Camera")) {
            auto& camera = *entityTable["Camera"].as_table();
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
}

} // namespace prism