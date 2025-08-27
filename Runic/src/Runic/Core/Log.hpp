#pragma once

#include "spdlog/logger.h"

namespace Runic
{

class Log
{
public:
    static void Init(const std::string& clientName);

    static std::shared_ptr<spdlog::logger>& Core() { return CoreLogger; }
    static std::shared_ptr<spdlog::logger>& Client() { return ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> CoreLogger;
    static std::shared_ptr<spdlog::logger> ClientLogger;
};

namespace Core
{

#ifdef NDEBUG
constexpr bool Debugging = false;
#else
constexpr bool Debugging = true;
#endif

template <typename Fmt, typename... Args>
void Trace(Fmt fmt, Args... args)
{
    if constexpr (Debugging) ::Runic::Log::Core()->trace(fmt::runtime(fmt), args...);
}

template <typename Fmt, typename... Args>
void Info(Fmt fmt, Args... args)
{
    if constexpr (Debugging) ::Runic::Log::Core()->info(fmt::runtime(fmt), args...);
}

template <typename Fmt, typename... Args>
void Warn(Fmt fmt, Args... args)
{
    if constexpr (Debugging) ::Runic::Log::Core()->warn(fmt::runtime(fmt), args...);
}

template <typename Fmt, typename... Args>
void Error(Fmt fmt, Args... args)
{
    if constexpr (Debugging) ::Runic::Log::Core()->error(fmt::runtime(fmt), args...);
}

template <typename Fmt, typename... Args>
void Critical(Fmt fmt, Args... args)
{
    if constexpr (Debugging) ::Runic::Log::Core()->critical(fmt::runtime(fmt), args...);
}

} // namespace Core

template <typename Fmt, typename... Args>
void Trace(Fmt fmt, Args... args)
{
    if constexpr (Core::Debugging) ::Runic::Log::Client()->trace(fmt::runtime(fmt), args...);
}

template <typename Fmt, typename... Args>
void Info(Fmt fmt, Args... args)
{
    if constexpr (Core::Debugging) ::Runic::Log::Client()->info(fmt::runtime(fmt), args...);
}

template <typename Fmt, typename... Args>
void Warn(Fmt fmt, Args... args)
{
    if constexpr (Core::Debugging) ::Runic::Log::Client()->warn(fmt::runtime(fmt), args...);
}

template <typename Fmt, typename... Args>
void Error(Fmt fmt, Args... args)
{
    if constexpr (Core::Debugging) ::Runic::Log::Client()->error(fmt::runtime(fmt), args...);
}

template <typename Fmt, typename... Args>
void Critical(Fmt fmt, Args... args)
{
    if constexpr (Core::Debugging) ::Runic::Log::Client()->critical(fmt::runtime(fmt), args...);
}

} // namespace Runic
