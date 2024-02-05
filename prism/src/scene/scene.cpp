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

}

void Scene::OnRender() {
    auto view = m_Registry.view<TransformComponent, SpriteRenderComponent>();

    for (auto [entity, transform, sprite] : view.each()) {
        Renderer2D::DrawQuad(transform, sprite.Color);
    }
}

Entity Scene::CreateEntity() {
    Entity entity =  { m_Registry.create(), this };
    entity.AddComponent<TransformComponent>(glm::mat4(1.0f));
    return entity;
}
} // namespace prism