#pragma once

#include "renderer/camera.hpp"
#include "renderer/texture.hpp"

namespace prism {

class Renderer2D {
public: 
    static void Init();
    static void Shutdown();

    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();

    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture);
};

} // namespace prism