#include "platform/Windows/windows_window.hpp"
#include "core/assert.hpp"
#include "core/event/application_event.hpp"
#include "core/event/key_event.hpp"
#include "core/event/mouse_event.hpp"
#include "platform/OpenGL/opengl_context.hpp"
#include "core/core.hpp"

namespace prism {

static void GLFWErrorCallback(int error, const char* description) {
    LOG_ERROR(log_tag::Window, "GLFW Error (", error, "): ", description);
}

Window* Window::Create(const WindowProps& props) {
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) {
    PRISM_PROFILE_FUNCTION();

    Init(props);
}

void WindowsWindow::Init(const WindowProps& props) {
    PRISM_PROFILE_FUNCTION();

    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;
    m_Data.vSync = true;

    LOG_INFO("EntryPoint", "==> ", props.title, " <==");
    
    {
        PRISM_PROFILE_SCOPE("glfwInit");
        PRISM_ASSERT(glfwInit(), "Failed to initialize GLFW", log_tag::Window);
        glfwSetErrorCallback(GLFWErrorCallback);
    }
    {
        PRISM_PROFILE_SCOPE("glfwCreateWindow");
        
        m_Window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
    }
    PRISM_ASSERT(m_Window, "Failed to create GLFW window", log_tag::Window);

    m_Context = new OpenGLContext(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(m_Data.vSync);

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.width = width;
        data.height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
        data.EventCallback(event);
    });
    
    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
        data.EventCallback(event);
    });
}

WindowsWindow::~WindowsWindow() {
    Shutdown();
}

void WindowsWindow::Shutdown() {
    PRISM_PROFILE_FUNCTION();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void WindowsWindow::OnUpdate() {
    PRISM_PROFILE_FUNCTION();

    glfwPollEvents();
    m_Context->SwapBuffers();
}
    
uint32_t WindowsWindow::GetWidth() const {
    return m_Data.width;
}

uint32_t WindowsWindow::GetHeight() const {
    return m_Data.height;
}

void WindowsWindow::SetEventCallback(const EventCallbackFn& callback) {
    m_Data.EventCallback = callback;
}

void WindowsWindow::SetVSync(bool enabled) {
    PRISM_PROFILE_FUNCTION();

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