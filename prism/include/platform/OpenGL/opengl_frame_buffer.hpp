#pragma once

#include "renderer/frame_buffer.hpp"

namespace prism {

class OpenGLFrameBuffer : public FrameBuffer {
public:
    OpenGLFrameBuffer(const FrameBufferSpecification& spec);
    virtual ~OpenGLFrameBuffer();

    virtual FrameBufferSpecification& GetSpecification() override { return m_Specification; }
    virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

    void Invalidate();    
private:
    uint32_t m_RendererID;
    uint32_t m_ColorAttachment, m_DepthAttachment;
    FrameBufferSpecification m_Specification;
};

} // namespace prism