#pragma once

#include <utility>

namespace prism {

class Input {
public:
    static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); };
    static bool IsKeyReleased(int keycode) { return s_Instance->IsKeyReleasedImpl(keycode); };
    static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); };
    static bool IsMouseButtonReleased(int button) { return s_Instance->IsMouseButtonReleasedImpl(button); };
    static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); };
    static float GetMouseX() { return s_Instance->GetMouseXImpl(); };
    static float GetMouseY() { return s_Instance->GetMouseYImpl(); };
protected:
    virtual bool IsKeyPressedImpl(int keycode) = 0;
    virtual bool IsKeyReleasedImpl(int keycode) = 0;
    virtual bool IsMouseButtonPressedImpl(int button) = 0;
    virtual bool IsMouseButtonReleasedImpl(int button) = 0;
    virtual std::pair<float, float> GetMousePositionImpl() = 0;
    virtual float GetMouseXImpl() = 0;
    virtual float GetMouseYImpl() = 0;
private:
    static Input* s_Instance;
};

} // namespace prism