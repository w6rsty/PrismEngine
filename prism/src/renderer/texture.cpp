#include "renderer/texture.hpp"
#include "renderer/renderer.hpp"
#include "platform/OpenGL/opengl_texture.hpp"

namespace prism {

Ref<Texture> Texture2D::Create(uint32_t width, uint32_t height) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: PRISM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
    }
};

Ref<Texture> Texture2D::Create(const std::string &path) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: PRISM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
    }
};

} // namespace prism