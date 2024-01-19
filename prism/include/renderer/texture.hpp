#pragma once

#include "core/core.hpp"

#include <cstdint>
#include <cstdint>
#include <string>

namespace prism {

class Texture {
public:
    virtual ~Texture() {};

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind(uint32_t slot  = 0) const = 0;
};

class Texture2D : public Texture {
public:
    static Ref<Texture> Create(const std::string& path);
};

} // namespace prism