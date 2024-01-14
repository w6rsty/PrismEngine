#include "platform/Windows/windows_input.hpp"

#include "application/application.hpp"
#include "GLFW/glfw3.h"

namespace prism {

bool WindowsInput::IsKeyPressedImpl(int keycode) {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsKeyReleasedImpl(int keycode) {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

    auto state = glfwGetKey(window, keycode);
    return state == GLFW_RELEASE;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button) {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

bool WindowsInput::IsMouseButtonReleasedImpl(int button) {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());
    
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_RELEASE;
}

std::pair<float, float> WindowsInput::GetMousePositionImpl() {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { static_cast<float>(xpos), static_cast<float>(ypos) };
}

float WindowsInput::GetMouseXImpl() {
    auto [x, y] = GetMousePositionImpl();
    return x;
}

float WindowsInput::GetMouseYImpl() {
    auto [x, y] = GetMousePositionImpl();
    return y;
}

} // namespace prism
