#include "renderer/renderer.hpp"
#include "platform/OpenGL/opengl_shader.hpp"
#include "renderer/renderer2d.hpp"

#include <memory>

namespace prism {

Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

void Renderer::Init() {
    RenderCommand::Init();
    Renderer2D::Init();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(OrthographicCamera& camera) {
    m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {

}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
    shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", transform);
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}

} // namespace prism