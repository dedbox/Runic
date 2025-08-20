#pragma once

#include "Input/Key.hpp"
#include "Input/Mouse.hpp"

namespace Runic
{

class Input
{
public:
    static bool IsKeyPressed(Key key);
    static bool IsMouseButtonPressed(MouseButton button);
};

} // namespace Runic
