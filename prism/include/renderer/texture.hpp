#pragma once

#include "core/core.hpp"

#include <cstdint>
#include <cstdint>
#include <string>

namespace prism {

enum class TextureWrap {
    None = 0,
    Repeat = 1,
    MirroredRepeat = 2,
    ClampToEdge = 3
};

class Texture {
public:
    virtual ~Texture() {};

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetWrap(TextureWrap wrap) = 0;

    virtual void SetData(void* data, uint32_t size) = 0;
    virtual void Bind(uint32_t slot  = 0) const = 0;

    virtual bool operator==(const Texture& other) = 0;
};

class Texture2D : public Texture {
public:
    static Ref<Texture> Create(uint32_t width, uint32_t height);
    static Ref<Texture> Create(const std::string& path);
};

} // namespace prism