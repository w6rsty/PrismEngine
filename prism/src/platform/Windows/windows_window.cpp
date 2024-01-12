#include "platform/Windows/windows_window.hpp"
#include "prism.hpp"
#include "GLFW/glfw3.h"

namespace prism {

Window* Window::Create(const WindowProps& props) {
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
    Init(props);
}

void WindowsWindow::Init(const WindowProps& props) {
    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;
    m_Data.vSync = true;

    LOG_INFO(log_tag::Window, props.title , " ", props.width, " ", props.height);

    PRISM_ASSERT(glfwInit(), "Failed to initialize GLFW", log_tag::Window);
    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    m_Window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(m_Data.vSync);
}

WindowsWindow::~WindowsWindow() {
    Shutdown();
}

void WindowsWindow::Shutdown() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void WindowsWindow::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}
    
unsigned int WindowsWindow::GetWidth() const {
    return m_Data.width;
}

unsigned int WindowsWindow::GetHeight() const {
    return m_Data.height;
}

void WindowsWindow::SetEventCallback(const EventCallbackFn& callback) {
    m_Data.EventCallback = callback;
}

void WindowsWindow::SetVSync(bool enabled) {
    m_Data.vSync = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
}

bool WindowsWindow::IsVSync() const {
    return m_Data.vSync;
}

void* WindowsWindow::GetNativeWindow() const {
    return m_Window;
}

} // namespace prism