#pragma once

#include "core/window/input.hpp"

namespace prism {

class WindowsInput final : public Input {
protected:
    virtual bool IsKeyPressedImpl(int keycode) override;
    virtual bool IsKeyReleasedImpl(int keycode) override;
    virtual bool IsMouseButtonPressedImpl(int button) override;
    virtual bool IsMouseButtonReleasedImpl(int button) override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
};

} // namespace prism