#pragma once

#include "pch.hpp"
#include "Runic/Event/Event.hpp"

namespace Runic
{
class RUNIC_API KeyEvent : public Event
{
public:
    int GetKeyCode() const { return _keyCode; }

    EventCategory category() const override { return EventCategory::Keyboard | EventCategory::Input; }

protected:
    explicit KeyEvent(const int keyCode)
        : _keyCode(keyCode) {}

    int _keyCode;
};

class RUNIC_API KeyPressedEvent final : public KeyEvent
{
public:
    KeyPressedEvent(const int keyCode, const bool repeated)
        : KeyEvent(keyCode), _repeated(repeated) {}

    bool isRepeated() const { return _repeated; }

    std::string toString() const override
    {
        return std::format("KeyPressedEvent: {}{}", _keyCode, _repeated ? " (repeat)" : "");
    }

    EventType staticType() const override { return EventType::KeyPressed; }

    const char* name() const override { return "KeyPressed"; }

private:
    bool _repeated;
};

class RUNIC_API KeyReleasedEvent final : public KeyEvent
{
public:
    explicit KeyReleasedEvent(const int keyCode)
        : KeyEvent(keyCode) {}

    std::string toString() const override
    {
        return std::format("KeyReleasedEvent: {}", _keyCode);
    }

    EventType staticType() const override { return EventType::KeyReleased; }

    const char* name() const override { return "KeyReleased"; }
};
}
