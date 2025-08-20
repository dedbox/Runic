#include "Input.hpp"

#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_mouse.h"

namespace Runic
{

bool Input::IsKeyPressed(Key key)
{
    auto modstate{SDL_GetModState()};
    const auto keycode{static_cast<SDL_Keycode>(key)};
    const auto scancode{SDL_GetScancodeFromKey(keycode, &modstate)};

    int numkeys{};
    const bool* raw_state{SDL_GetKeyboardState(&numkeys)};
    std::span<const bool> state(raw_state, numkeys);

    return state[scancode];
}

bool Input::IsMouseButtonPressed(MouseButton button)
{
    float x{}, y{};
    const SDL_MouseButtonFlags flags{SDL_GetMouseState(&x, &y)};
    const auto flag{static_cast<SDL_MouseButtonFlags>(button)};

    return flags & SDL_BUTTON_MASK(flag);
}

} // namespace Runic
