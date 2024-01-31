#include "renderer/frame_buffer.hpp"

#include "renderer/renderer.hpp"
#include "platform/OpenGL/opengl_frame_buffer.hpp"

namespace prism {

Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        }
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
    }

    return nullptr;
}

} // namespace prism