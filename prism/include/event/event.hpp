#pragma once

#include <ostream>
#include <string>
#include <functional>
#include <type_traits>

namespace prism {

#define BIT(x) (1 << (x))

enum class EventType {
    None = 0,
    WindowClosed, WindowResized, WindowForcus, WindowLostForcus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,  
};

enum EventCategory {
    None = 0,
    EventCategoryApplication    = BIT(0),
    EventCategoryInput          = BIT(1),
    EventCategoryKeyboard       = BIT(2),
    EventCategoryMouse          = BIT(3),
    EventCategoryMouseButton    = BIT(4)
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; } \
                               virtual EventType GetEventType() const override { return GetStaticType(); } \
                               virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Event {
public:
    bool m_Handled = { false };

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); } 

    inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
};

class EventDispatcher {
private:
    Event& m_Event;
public:
    template <typename T>
    using EventFunc = std::function<bool(T&)>;

    EventDispatcher(Event& event)
        : m_Event(event) {}
    
    template <typename T>
    bool Dispatch(EventFunc<T> func) {
        if (m_Event.GetEventType() == T::GetStaticType()) {
            m_Event.m_Handled = func(*(T*)&m_Event);
            return true;
        }
        return false;
    }
};

inline std::ostream& operator<<(std::ostream& stream, const Event& event) {
    return stream << event.ToString();
}

} // namespace prism