#include "platform/OpenGL/opengl_buffer.hpp"

#include "glad/glad.h"

namespace prism {

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
    PRISM_PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    PRISM_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind() const {
    PRISM_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const {
    PRISM_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)

: m_Count(count) {
    PRISM_PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    PRISM_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind() const {
    PRISM_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
    PRISM_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace prism