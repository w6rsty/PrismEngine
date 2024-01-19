#include "renderer/vertex_array.hpp"
#include "core/assert.hpp"
#include "renderer/renderer.hpp"
#include "platform/OpenGL/opengl_vertex_array.hpp"

namespace prism {

Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLVertexArray>();
        }
    }
}

} // namespace prism