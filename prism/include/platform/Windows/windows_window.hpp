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
    
    virtual unsigned int GetWidth() const override;
    virtual unsigned int GetHeight() const override;

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
        unsigned int width;
        unsigned int height;
        bool vSync;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};


} // namespace prism