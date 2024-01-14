#pragma once

#include "event.hpp"
#include <sstream>

namespace prism {

class KeyEvent : public Event {
protected:
    int m_KeyCode;

    KeyEvent(int keycode) : m_KeyCode(keycode) {}
public:
    inline int GetKeyCode() const { return m_KeyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
};

class KeyPressedEvent final : public KeyEvent {
private:
    int m_RepeatCount;
public:
    KeyPressedEvent(int keycode, int repeatCount)
        : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

    inline int GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressed: " << m_KeyCode << "(" << m_RepeatCount <<" repeats)"; 
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
};

class KeyReleasedEvent final : public KeyEvent {
public:
    KeyReleasedEvent(int keycode)
        : KeyEvent(keycode) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleased: " << m_KeyCode; 
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent final : public KeyEvent {
public:
    KeyTypedEvent(int keycode)
        : KeyEvent(keycode) {}

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleased: " << m_KeyCode; 
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};

} // namespace prism