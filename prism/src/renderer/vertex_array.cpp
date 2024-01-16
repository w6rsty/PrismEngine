#include "renderer/vertex_array.hpp"
#include "core/assert.hpp"
#include "renderer/renderer.hpp"
#include "platform/OpenGL/opengl_vertex_array.hpp"

namespace prism {

VertexArray* VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::OpenGL: {
            return new OpenGLVertexArray();
        }
        case RendererAPI::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
    }
}

} // namespace prism