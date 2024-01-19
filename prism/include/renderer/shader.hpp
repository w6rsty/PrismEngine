#pragma once

#include "core/core.hpp"

#include <string>

namespace prism {

class Shader {
public:
    virtual ~Shader() {};

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
};

} // namespace prism