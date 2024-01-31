#pragma once

#include "core/window/window.hpp"
#include "renderer/graphics_context.hpp"
#include "GLFW/glfw3.h"

namespace prism {

// GLFW Window
class WindowsWindow : public Window {
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    virtual void OnUpdate() override;
    
    virtual uint32_t GetWidth() const override;
    virtual uint32_t GetHeight() const override;

    virtual void SetEventCallback(const EventCallbackFn& callback) override;
    virtual void SetVSync(bool enabled) override;
    virtual bool IsVSync() const override;

    virtual void* GetNativeWindow() const override;
private:
    void Init(const WindowProps& props);
    void Shutdown();
private:
    GLFWwindow* m_Window;
    GraphicsContext* m_Context;

    struct WindowData {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool vSync;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};


} // namespace prism