#pragma once

#include <string>
#include <functional>
#include "event/event.hpp"

namespace prism {

struct WindowProps {
    std::string title;
    unsigned int width;
    unsigned int height;

    WindowProps(const std::string& title = "PrismEngine",   
                unsigned int w = 1280, 
                unsigned int h = 800)
    : title(title), width(w), height(h) {}
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event&)>
;    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;
    
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const WindowProps& props = WindowProps{});
};

} // namespace prism