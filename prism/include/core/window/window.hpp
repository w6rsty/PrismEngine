#pragma once

#include <string>
#include <functional>
#include "core/event/event.hpp"

namespace prism {

struct WindowProps {
    std::string title;
    uint32_t width;
    uint32_t height;

    WindowProps(const std::string& title = "PrismEngine",   
                uint32_t w = 1920, 
                uint32_t h = 1080)
    : title(title), width(w), height(h) {}
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event&)>
;    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const WindowProps& props = WindowProps{});
};

} // namespace prism