#include "platform/OpenGL/opengl_context.hpp"

#include "glad/glad.h"
#include "core/log_tag.hpp"
#include "core/assert.hpp"

#include "GLFW/glfw3.h"

namespace prism {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
: m_WindowHandle(windowHandle) {
    PRISM_ASSERT(m_WindowHandle, "Window handle is null", log_tag::Renderer);
}

void OpenGLContext::Init() {
    glfwMakeContextCurrent(m_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    PRISM_ASSERT(status, "Failed to initialize Glad", log_tag::Window);
    LOG_INFO("Vendor", glGetString(GL_VENDOR));
    LOG_INFO("Device", glGetString(GL_RENDERER));
    LOG_INFO("Version", glGetString(GL_VERSION));
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
}

} // namespace prism