#pragma once

#include "event.hpp"
#include <sstream>

namespace prism {

class WindowResizeEvent final : public Event {
private:
    unsigned int m_Width, m_Height;
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height) {}
    inline unsigned int GetWidth() const { return m_Width; }
    inline unsigned int GetHeight() const { return m_Height; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "WindowResize: " << m_Width << ", " << m_Height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResized)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class WindowCloseEvent final : public Event {
public:
    WindowCloseEvent() {}

    EVENT_CLASS_TYPE(WindowClosed)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent final : public Event {
public:
    AppTickEvent() {}

    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent final : public Event {
public:
    AppUpdateEvent() {}

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent final : public Event {
public:
    AppRenderEvent() {}

    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

} // namespace prism