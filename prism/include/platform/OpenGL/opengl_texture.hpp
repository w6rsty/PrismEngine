#pragma once

#include "renderer/texture.hpp"
#include "glad/glad.h"

#include <cstdint>

namespace prism {

class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    OpenGLTexture2D(const std::string& path);
    virtual ~OpenGLTexture2D();

    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override { return m_Height; }
    virtual uint32_t GetRendererID() const override { return m_RendererID; }
    
    virtual void SetData(void* data, uint32_t size) override;
    virtual void SetWrap(TextureWrap wrap) override;

    virtual void Bind(uint32_t slot = 0) const override;

    virtual bool operator==(const Texture& other) override;
private:
    uint32_t m_RendererID;
    uint32_t m_Width, m_Height;
    std::string m_Path;
    GLenum m_InternalFormat, m_DataFormat;
};

} // namespace prism