#pragma once

#include "core/core.hpp"

namespace prism {

struct FrameBufferSpecification {
    uint32_t width, height;
    uint32_t samples = 1;

    bool swapChainTarget = false;
};

class FrameBuffer {
public:
    virtual ~FrameBuffer() = default;

    virtual FrameBufferSpecification& GetSpecification() = 0;
    virtual const FrameBufferSpecification& GetSpecification() const = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual uint32_t GetColorAttachmentRendererID() const = 0;

    static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
};

} // namespace prism