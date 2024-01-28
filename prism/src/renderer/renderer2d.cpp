#include "renderer/renderer2d.hpp"

#include "core/core.hpp"
#include "core/logger.hpp"
#include "renderer/shader.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/render_command.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include <cstdint>
#include <array>

namespace prism {
    
struct QuadVertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    float texIndex;
    float tilingFactor;
};

struct Render2DData {
    const uint32_t maxQuads = 10000;
    const uint32_t maxVertices = maxQuads * 4;
    const uint32_t maxIndices = maxQuads * 6;
    static const uint32_t maxTextureSlots = 32;

    Ref<VertexArray> quadVertexArray;
    Ref<VertexBuffer> quadVertexBuffer;
    Ref<Shader> shader;
    Ref<Texture> whiteTexture;

    uint32_t quadIndexCount = 0;
    QuadVertex* quadVertexBufferBase = nullptr;
    QuadVertex* quadVertexBufferPtr = nullptr;

    std::array<Ref<Texture>, maxTextureSlots> textureSlots;
    uint32_t textureSlotIndex = 1;

    glm::vec4 quadVertexPositions[4];
};

static Render2DData s_Data;


void Renderer2D::Init() {
    PRISM_PROFILE_FUNCTION();

    s_Data.quadVertexArray = VertexArray::Create();

    {
        PRISM_PROFILE_SCOPE("Renderer2D::Init::VertexBuffer");        
        s_Data.quadVertexBuffer = VertexBuffer::Create(s_Data.maxVertices * sizeof(QuadVertex));
        s_Data.quadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float, "a_TexIndex" },
            { ShaderDataType::Float, "a_TilingFactor" }
        });
        s_Data.quadVertexArray->AddVertexBuffer(s_Data.quadVertexBuffer);
    }

    {
        PRISM_PROFILE_SCOPE("Renderer2D::Init::IndexBuffer");
        s_Data.quadVertexBufferBase = new QuadVertex[s_Data.maxVertices];
        uint32_t* quadIndices = new uint32_t[s_Data.maxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.maxIndices; i+=6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }
        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.maxIndices);
        s_Data.quadVertexArray->SetIndexBuffer(quadIB);
        delete [] quadIndices;
    }

    s_Data.whiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
    s_Data.textureSlots[0] = s_Data.whiteTexture;

    int samplers[s_Data.maxTextureSlots];
    for (uint32_t i = 0; i < s_Data.maxTextureSlots; i++) {
        samplers[i] = i;
    }

    s_Data.shader = Shader::Create("../../assets/shaders/shader.glsl");
    s_Data.shader->Bind();
    s_Data.shader->SetIntArray("u_Textures", samplers, s_Data.maxTextureSlots);

    s_Data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
    s_Data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
}

void Renderer2D::Shutdown() {
    PRISM_PROFILE_FUNCTION();

}

void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    PRISM_PROFILE_FUNCTION();

    s_Data.shader->Bind();
    s_Data.shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    s_Data.quadIndexCount = 0;
    s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;

    s_Data.textureSlotIndex = 1;
}

void Renderer2D::EndScene() {
    PRISM_PROFILE_FUNCTION();

    uint32_t dataSize = (uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase;
    s_Data.quadVertexBuffer->SetData(s_Data.quadVertexBufferBase, dataSize);

    Flush();
}

void Renderer2D::Flush() {
    PRISM_PROFILE_FUNCTION();

    for (uint32_t i = 0; i < s_Data.textureSlotIndex; i++) {
        s_Data.textureSlots[i]->Bind(i);
    }

    RenderCommand::DrawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    PRISM_PROFILE_FUNCTION();
    
    constexpr float textureIndex = 0.0f;

    float tilingFactor = 1.0f;

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[0];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[1];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[2];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[3];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadIndexCount += 6;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
}


void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tiling, const glm::vec4& tintColor) {
    PRISM_PROFILE_FUNCTION();
    
    constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    float textureIndex = 0.0f;
    float tilingFactor = tiling;

    for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++) {
        if (*s_Data.textureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        textureIndex = (float)s_Data.textureSlotIndex;
        s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
        s_Data.textureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[0];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[1];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[2];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[3];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadIndexCount += 6;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture>& texture, float tiling, const glm::vec4& tintColor) {
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling, tintColor);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
    PRISM_PROFILE_FUNCTION();
    
    constexpr float textureIndex = 0.0f;

    float tilingFactor = 1.0f;

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[0];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[1];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[2];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[3];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadIndexCount += 6;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tiling, const glm::vec4& tintColor) {
    PRISM_PROFILE_FUNCTION();
    
    constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    float textureIndex = 0.0f;
    float tilingFactor = tiling;

    for (uint32_t i = 1; i < s_Data.textureSlotIndex; i++) {
        if (*s_Data.textureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        textureIndex = (float)s_Data.textureSlotIndex;
        s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
        s_Data.textureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
    * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
    * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[0];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[1];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[2];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[3];
    s_Data.quadVertexBufferPtr->color = color;
    s_Data.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
    s_Data.quadVertexBufferPtr->texIndex = textureIndex;
    s_Data.quadVertexBufferPtr->tilingFactor = tilingFactor;
    s_Data.quadVertexBufferPtr++;

    s_Data.quadIndexCount += 6;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture>& texture, float tiling, const glm::vec4& tintColor) {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling, tintColor);
}

} // namespace prism