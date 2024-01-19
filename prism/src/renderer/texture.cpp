#include "renderer/texture.hpp"
#include "renderer/renderer.hpp"
#include "platform/OpenGL/opengl_texture.hpp"

namespace prism {

Ref<Texture> Texture2D::Create(const std::string &path) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: PRISM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
    }
};

} // namespace prism