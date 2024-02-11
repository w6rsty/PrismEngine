#pragma once

#include "renderer/camera.hpp"
#include "renderer/texture.hpp"
#include "renderer/subtexture2d.hpp"

namespace prism {

class Renderer2D {
public: 
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& camera, const glm::mat4& transform);
    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();
    static void Flush();

    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));    

    static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

    static void DrawTexture(const glm::vec3& position, const Ref<Texture>& texture, float scale = 1.0f, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
    static void DrawTexture(const glm::vec2& position, const Ref<Texture>& texture, float scale = 1.0f, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

    struct Statistics {
        uint32_t drawCalls = 0;
        uint32_t quadCount = 0;

        uint32_t GetTotalVertexCount() { return quadCount * 4; }
        uint32_t GetTotalIndexCount() { return quadCount * 6; }
    };

    static Statistics GetStats();
    static void ReSetStats();
private:
    static void StartBatch();
    static void NextBatch();
};

} // namespace prism