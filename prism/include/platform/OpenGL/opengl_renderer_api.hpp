#pragma once

#include "renderer/renderer_api.hpp"

namespace prism {

class OpenGLRendererAPI : public RendererAPI {
public:
    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;

    virtual void DrawIndewed(const std::shared_ptr<VertexArray>& vertexArray) override;
};

} // namespace prism