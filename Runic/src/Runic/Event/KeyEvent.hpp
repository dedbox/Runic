#pragma once

#include "pch.hpp"
#include "Runic/Input/Key.hpp"
#include "Runic/Event/Event.hpp"

namespace Runic
{
class RUNIC_API KeyEvent : public Event
{
public:
    Key keyCode() const { return static_cast<Key>(_keyCode); }

    EventCategory category() const override { return EventCategory::Keyboard | EventCategory::Input; }

protected:
    explicit KeyEvent(const int keyCode)
        : _keyCode(keyCode) {}

    int _keyCode;
};

class RUNIC_API KeyPressedEvent final : public KeyEvent
{
public:
    KeyPressedEvent(const int keyCode, const int scanCode, const bool repeated)
        : KeyEvent(keyCode), _scanCode(scanCode), _repeated(repeated) {}

    int scanCode() const { return _scanCode; }

    bool isRepeated() const { return _repeated; }

    std::string toString() const override
    {
        return std::format("KeyPressedEvent: {} {}{}", _keyCode, _scanCode, _repeated ? " (repeat)" : "");
    }

    static EventType staticType() { return EventType::KeyPressed; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "KeyPressed"; }

private:
    int _scanCode;
    bool _repeated;
};

class RUNIC_API KeyReleasedEvent final : public KeyEvent
{
public:
    explicit KeyReleasedEvent(const int keyCode, const int scanCode)
        : KeyEvent(keyCode), _scanCode(scanCode) {}

    int scanCode() const { return _scanCode; }

    std::string toString() const override
    {
        return std::format("KeyReleasedEvent: {} {}", _keyCode, _scanCode);
    }

    static EventType staticType() { return EventType::KeyReleased; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "KeyReleased"; }

private:
    int _scanCode;
};

class RUNIC_API KeyTypedEvent final : public KeyEvent
{
public:
    explicit KeyTypedEvent(const int keyCode)
        : KeyEvent(keyCode) {}

    std::string toString() const override
    {
        return std::format("KeyTypedEvent: {}", _keyCode);
    }

    static EventType staticType() { return EventType::KeyTyped; }

    EventType type() const override { return staticType(); }

    const char* name() const override { return "KeyTyped"; }
};
}
