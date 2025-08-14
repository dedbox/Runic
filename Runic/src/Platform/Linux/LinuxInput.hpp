#pragma once

#include "Runic/Input.hpp"

namespace Runic
{
class LinuxInput final : public Input
{
public:
    bool isKeyPressedImpl(int keyCode) override;

    bool isMouseButtonPressedImpl(int button) override;

    std::pair<float, float> getMousePositionImpl() override;
};
} // Runic
