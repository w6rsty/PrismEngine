#include "core//window/input.hpp"
#include "core/application.hpp"

#include "GLFW/glfw3.h"

namespace prism {

bool Input::IsKeyPressed(int keycode) {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsKeyReleased(int keycode) {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

    auto state = glfwGetKey(window, keycode);
    return state == GLFW_RELEASE;
}

bool Input::IsMouseButtonPressed(int button) {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

bool Input::IsMouseButtonReleased(int button) {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());
    
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_RELEASE;
}

std::pair<float, float> Input::GetMousePosition() {
    auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { static_cast<float>(xpos), static_cast<float>(ypos) };
}

float Input::GetMouseX() {
    auto [x, y] = GetMousePosition();
    return x;
}

float Input::GetMouseY() {
    auto [x, y] = GetMousePosition();
    return y;
}

} // namespace prism
