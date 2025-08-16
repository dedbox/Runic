#pragma once

#include "Runic/Core.hpp"
#include "Runic/Input/Key.hpp"
#include "Runic/Input/MouseButton.hpp"

namespace Runic
{
class RUNIC_API Input
{
public:
    virtual ~Input() = default;

    static bool isKeyPressed(const Key keyCode) { return s_instance->isKeyPressedImpl(keyCode); }

    static bool isMouseButtonPressed(const MouseButton button) { return s_instance->isMouseButtonPressedImpl(button); }

    static std::pair<float, float> getMousePosition() { return s_instance->getMousePositionImpl(); }

protected:
    virtual bool isKeyPressedImpl(Key keyCode) = 0;

    virtual bool isMouseButtonPressedImpl(MouseButton button) = 0;

    virtual std::pair<float, float> getMousePositionImpl() = 0;

private:
    static Input* s_instance;
};
}
