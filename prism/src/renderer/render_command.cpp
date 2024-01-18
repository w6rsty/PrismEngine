#include "renderer/render_command.hpp"
#include "platform/OpenGL/opengl_renderer_api.hpp"

namespace prism {

RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

} // namespace prism