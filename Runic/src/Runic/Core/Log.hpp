#pragma once

#include "Runic/Core.hpp"

#include "spdlog/logger.h"

namespace Runic
{

class Log
{
public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& Core() { return CoreLogger; }
    static std::shared_ptr<spdlog::logger>& Client() { return ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> CoreLogger;
    static std::shared_ptr<spdlog::logger> ClientLogger;
};

} // namespace Runic

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
// clang-format off
#define RUNIC_CORE_TRACE(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Core()->trace(__VA_ARGS__)
#define RUNIC_CORE_INFO(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Core()->info(__VA_ARGS__)
#define RUNIC_CORE_WARN(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Core()->warn(__VA_ARGS__)
#define RUNIC_CORE_ERROR(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Core()->error(__VA_ARGS__)
#define RUNIC_CORE_CRITICAL(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Core()->critical(__VA_ARGS__)

#define RUNIC_TRACE(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Client()->trace(__VA_ARGS__)
#define RUNIC_INFO(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Client()->info(__VA_ARGS__)
#define RUNIC_WARN(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Client()->warn(__VA_ARGS__)
#define RUNIC_ERROR(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Client()->error(__VA_ARGS__)
#define RUNIC_CRITICAL(...) if constexpr(::Runic::Core::Debugging) ::Runic::Log::Client()->critical(__VA_ARGS__)
// clang-format on

#define RUNIC_CORE_ASSERT(x, fmt, ...)                                                             \
    if constexpr (::Runic::Core::Debugging) {                                                      \
        if (!(x)) {                                                                                \
            RUNIC_CORE_ERROR("Assertion failed: " fmt, __VA_ARGS__);                               \
            if (!raise(SIGTRAP))                                                                   \
                throw std::runtime_error("trap failed");                                           \
        }                                                                                          \
    }

#define RUNIC_ASSERT(x, fmt, ...)                                                                  \
    if constexpr (::Runic::Core::Debugging) {                                                      \
        if (!(x)) {                                                                                \
            RUNIC_ERROR("Assertion failed: " fmt, __VA_ARGS__);                                    \
            if (!raise(SIGTRAP))                                                                   \
                throw std::runtime_error("trap failed");                                           \
        }                                                                                          \
    }
// NOLINTEND(cppcoreguidelines-macro-usage)
