#include "renderer/shader.hpp"

#include "platform/OpenGL/opengl_shader.hpp"
#include "renderer/renderer_api.hpp"

namespace prism {

Ref<Shader> Shader::Create(const std::string &vertexSrc, const std::string &fragmentSrc) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None: {
            PRISM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }

        case RendererAPI::API::OpenGL: {
            return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
        }
    }
}

} // namespace prism