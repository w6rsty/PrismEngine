#include "scene/scene.hpp"

#include "renderer/renderer2d.hpp"
#include "scene/components.hpp"

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

} // namespace prism