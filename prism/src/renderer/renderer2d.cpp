#include "renderer/renderer2d.hpp"

#include "core/logger.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/render_command.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace prism {
    
struct Render2DStorage {
    Ref<VertexArray> quadVertexArray;
    Ref<Shader> shader;
    Ref<Texture> whiteTexture;
};

static Render2DStorage* s_Data;

void Renderer2D::Init() {
    PRISM_PROFILE_FUNCTION();

    s_Data = new Render2DStorage();

    float square_vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // 3
    };
    BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float2, "a_TexCoord" },
    };
    uint32_t square_indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    s_Data->quadVertexArray = VertexArray::Create();

    Ref<VertexBuffer> squareVB;
    squareVB = VertexBuffer::Create(square_vertices, sizeof(square_vertices));
    squareVB->SetLayout(layout);
    s_Data->quadVertexArray->AddVertexBuffer(squareVB);
    Ref<IndexBuffer> squareIB = IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
    s_Data->quadVertexArray->SetIndexBuffer(squareIB);

    s_Data->whiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    s_Data->shader = Shader::Create("../../assets/shaders/shader.glsl");
    s_Data->shader->Bind();
    s_Data->shader->SetInt("u_Texture", 0);
}

void Renderer2D::Shutdown() {
    PRISM_PROFILE_FUNCTION();
    delete s_Data;
}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    PRISM_PROFILE_FUNCTION();

    s_Data->shader->Bind();
    s_Data->shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}

void Renderer2D::EndScene() {
    PRISM_PROFILE_FUNCTION();

}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    PRISM_PROFILE_FUNCTION();
    
    s_Data->shader->SetFloat4("u_Color", color);
    s_Data->whiteTexture->Bind();
    glm::mat4 transform(glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f)));
    s_Data->shader->SetMat4("u_Transform", transform);

    s_Data->quadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->quadVertexArray);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tiling) {
    PRISM_PROFILE_FUNCTION();
    
    s_Data->shader->SetFloat4("u_Color", glm::vec4(1.0f));
    s_Data->shader->SetFloat("u_TilingFactor", tiling);
    texture->Bind();
    glm::mat4 transform(glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f)));
    s_Data->shader->SetMat4("u_Transform", transform);

    s_Data->quadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->quadVertexArray);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tiling) {
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling);
}

} // namespace prism