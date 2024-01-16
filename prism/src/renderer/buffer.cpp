#include "renderer/buffer.hpp"
#include "renderer/renderer.hpp"
#include "core/assert.hpp"
#include "platform/OpenGL/opengl_buffer.hpp"

namespace prism {

VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        }
        case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
    }

    return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        }
        case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
    }

    return nullptr;
}

} // namespace prism