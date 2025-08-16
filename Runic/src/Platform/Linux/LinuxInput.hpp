#pragma once

#include "Runic/Input/Input.hpp"

namespace Runic
{
class LinuxInput final : public Input
{
public:
    bool isKeyPressedImpl(Key keyCode) override;

    bool isMouseButtonPressedImpl(MouseButton button) override;

    std::pair<float, float> getMousePositionImpl() override;
};
} // Runic
