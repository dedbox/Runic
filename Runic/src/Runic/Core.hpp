#pragma once

namespace Runic::Core
{

#ifdef NDEBUG
constexpr bool Debugging{false};
#else
constexpr bool Debugging{true};
#endif

} // namespace Runic::Core
