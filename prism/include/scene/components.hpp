#pragma once

#include "glm/glm.hpp"

namespace prism {

struct TransformComponent {
    glm::mat4 Transform;

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::mat4& transform)
        : Transform(transform) {
    }

    operator glm::mat4&() { return Transform; }
    operator const glm::mat4&() const { return Transform; }
};

struct SpriteRenderComponent {
    glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SpriteRenderComponent() = default;
    SpriteRenderComponent(const SpriteRenderComponent&) = default;
    SpriteRenderComponent(const glm::vec4& color)
        : Color(color) {
    }
};

} // namespace prism