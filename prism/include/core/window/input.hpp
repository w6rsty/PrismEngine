#pragma once

#include <utility>

namespace prism {

class Input {
public:
    static bool IsKeyPressed(int keycode) ;
    static bool IsKeyReleased(int keycod);
    static bool IsMouseButtonPressed(int button);
    static bool IsMouseButtonReleased(int button);
    static std::pair<float, float> GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};

} // namespace prism