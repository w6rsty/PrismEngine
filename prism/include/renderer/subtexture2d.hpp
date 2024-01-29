#pragma once

#include "core/core.hpp"
#include "texture.hpp"

#include "glm/glm.hpp"

namespace prism {

class SubTexture2D {
public:
    SubTexture2D(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max);

    const Ref<Texture> GetTexture() const { return m_Texture; }
    const glm::vec2* GetTexCoords() const { return m_TexCoords; }

    static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize);
private:
    Ref<Texture> m_Texture;
    glm::vec2 m_TexCoords[4];
};

} // namespace prism