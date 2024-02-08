#include "scene/scene.hpp"

#include "renderer/renderer2d.hpp"
#include "scene/components.hpp"
#include "scene/entity.hpp"

#include "glm/glm.hpp"

namespace prism {


Scene::Scene() {
    
}

Scene::~Scene() {

}

void Scene::OnUpdate(Timestep ts) {
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
            if (!nsc.Instance) {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = Entity{ entity, this };
                nsc.Instance->OnCreate();
            }

            nsc.Instance->OnUpdate(ts);
        });
    }

    Camera* mainCamera = nullptr;
    glm::mat4* camraTransform = nullptr;
    {
        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view) {
            const auto [transform, camera] = m_Registry.get<TransformComponent, CameraComponent>(entity);

            if (camera.primary) {
                mainCamera = &camera.camera;
                camraTransform = &transform.Transform;
                break;
            }
        }
    }
    if (mainCamera) {
        Renderer2D::BeginScene(*mainCamera, *camraTransform);
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);

        for (auto entity : group) {
            const auto [transform, sprite] = group.get<TransformComponent, SpriteRenderComponent>(entity);
            Renderer2D::DrawQuad(transform, sprite.Color);
        }

        Renderer2D::EndScene();
    }
}


Entity Scene::CreateEntity(const std::string& name) {
    Entity entity =  { m_Registry.create(), this };
    entity.AddComponent<TransformComponent>(glm::mat4(1.0f));
    entity.AddComponent<TagComponent>(name);
    return entity;
}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height; 

    auto view = m_Registry.view<CameraComponent>();

    for (auto entity : view) {
        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (!cameraComponent.fixedAspectRatio) {
            cameraComponent.camera.SetViewportSize(width, height);
        }
    }
}

} // namespace prism