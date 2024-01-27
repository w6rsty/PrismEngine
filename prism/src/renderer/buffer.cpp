#include "renderer/buffer.hpp"
#include "renderer/renderer.hpp"
#include "core/assert.hpp"
#include "platform/OpenGL/opengl_buffer.hpp"
#include <memory>

namespace prism {

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
    }

    return nullptr;
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
    }

    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
    }

    return nullptr;
}

} // namespace prism