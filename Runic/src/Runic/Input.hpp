#pragma once

#include "Runic/Core.hpp"

namespace Runic
{
class RUNIC_API Input
{
public:
    virtual ~Input() = default;

    static bool isKeyPressed(const int keyCode) { return s_instance->isKeyPressedImpl(keyCode); }

    static bool isMouseButtonPressed(const int button) { return s_instance->isMouseButtonPressedImpl(button); }

    static std::pair<float, float> getMousePosition() { return s_instance->getMousePositionImpl(); }

protected:
    virtual bool isKeyPressedImpl(int keyCode) = 0;

    virtual bool isMouseButtonPressedImpl(int button) = 0;

    virtual std::pair<float, float> getMousePositionImpl() = 0;

private:
    static Input* s_instance;
};
}
