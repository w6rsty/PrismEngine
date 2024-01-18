#pragma once

#include "renderer_api.hpp"

namespace prism {

class RenderCommand {
public:
    inline static void SetClearColor(const glm::vec4& color) {
        s_RendererAPI->SetClearColor(color);
    }
    inline static void Clear() {
        s_RendererAPI->Clear();
    }
    inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
        s_RendererAPI->DrawIndewed(vertexArray);
    }
private:
    static RendererAPI* s_RendererAPI;
};

} // namespace prism