#pragma once

#include <memory>

#include "spdlog/spdlog.h"

#include "Runic/Core.hpp"

namespace Runic
{
class RUNIC_API Log
{
public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
}

// Core log macros
#define RUNIC_CORE_TRACE(...) if constexpr (IS_DEBUG) ::Runic::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RUNIC_CORE_INFO(...)  if constexpr (IS_DEBUG) ::Runic::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RUNIC_CORE_WARN(...)  if constexpr (IS_DEBUG) ::Runic::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RUNIC_CORE_ERROR(...) if constexpr (IS_DEBUG) ::Runic::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RUNIC_CORE_FATAL(...) if constexpr (IS_DEBUG) ::Runic::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define RUNIC_TRACE(...) if constexpr (IS_DEBUG) ::Runic::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RUNIC_INFO(...)  if constexpr (IS_DEBUG) ::Runic::Log::GetClientLogger()->info(__VA_ARGS__)
#define RUNIC_WARN(...)  if constexpr (IS_DEBUG) ::Runic::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RUNIC_ERROR(...) if constexpr (IS_DEBUG) ::Runic::Log::GetClientLogger()->error(__VA_ARGS__)
#define RUNIC_FATAL(...) if constexpr (IS_DEBUG) ::Runic::Log::GetClientLogger()->fatal(__VA_ARGS__)

#define RUNIC_ASSERT(x, ...) if constexpr (IS_ASSERTED) {if(!(x)) {RUNIC_ERROR("Assertion failed: {}", __VA_ARGS__); raise(SIGTRAP); }}
#define RUNIC_CORE_ASSERT(x, ...) if constexpr (IS_ASSERTED) {if(!(x)) {RUNIC_CORE_ERROR("Assertion failed: {}", __VA_ARGS__); raise(SIGTRAP); }}
