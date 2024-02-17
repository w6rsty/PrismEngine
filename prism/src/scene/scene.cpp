#include "scene/scene.hpp"

#include "renderer/renderer2d.hpp"
#include "scene/components.hpp"
#include "scene/entity.hpp"

#include "glm/glm.hpp"

namespace prism {


Scene::Scene()
: m_Name("Untitled scene") {
    
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
    glm::mat4 camraTransform;
    {
        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view) {
            const auto [transform, camera] = m_Registry.get<TransformComponent, CameraComponent>(entity);

            if (camera.Primary) {
                mainCamera = &camera.Camera;
                camraTransform = transform.GetTransform();
                break;
            }
        }
    }
    if (mainCamera) {
        Renderer2D::BeginScene(*mainCamera, camraTransform);
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRenderComponent>);

        for (auto entity : group) {
            const auto [transform, sprite] = group.get<TransformComponent, SpriteRenderComponent>(entity);
            Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
        }

        Renderer2D::EndScene();
    }
}


Entity Scene::CreateEntity(const std::string& name) {
    Entity entity =  { m_Registry.create(), this };
    entity.AddComponent<TransformComponent>();
    entity.AddComponent<TagComponent>(name);
    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    m_Registry.destroy(entity);
}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height; 

    auto view = m_Registry.view<CameraComponent>();

    for (auto entity : view) {
        auto& cameraComponent = view.get<CameraComponent>(entity);
        if (!cameraComponent.FixedAspectRatio) {
            cameraComponent.Camera.SetViewportSize(width, height);
        }
    }
}

Entity Scene::GetPrimaryCameraEntity() {
    auto view = m_Registry.view<CameraComponent>();
    for (auto entity : view) {
        const auto& camera = view.get<CameraComponent>(entity);
        if (camera.Primary) {
            return Entity{ entity, this };
        }
    }
    return Entity{};
}

void Scene::OnComponentAdded(const std::string& name) {
    // TODO: Optimize this
    if (std::string(name) == typeid(TransformComponent).name()) {
        auto view = m_Registry.view<TransformComponent>();
        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
        }
    }

    if (std::string(name) == typeid(SpriteRenderComponent).name()) {
        auto view = m_Registry.view<SpriteRenderComponent>();
        for (auto entity : view) {
            auto& sprite = view.get<SpriteRenderComponent>(entity);
        }
    }

    if (std::string(name) == typeid(CameraComponent).name()) {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& camera = view.get<CameraComponent>(entity);
            camera.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
        }
    }

    if (std::string(name) == typeid(NativeScriptComponent).name()) {
        auto view = m_Registry.view<NativeScriptComponent>();
        for (auto entity : view) {
            auto& nsc = view.get<NativeScriptComponent>(entity);
        }
    }
}

} // namespace prism