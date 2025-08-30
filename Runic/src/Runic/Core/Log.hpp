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

    static void SetLevel(spdlog::level::level_enum level)
    {
        CoreLogger->set_level(level);
        ClientLogger->set_level(level);
    }

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

template <typename... Args>
void Trace(const std::string& fmt, Args... args)
{
    if constexpr (Debugging)
        ::Runic::Log::Core()->trace(fmt::runtime(fmt), args...);
}

template <typename... Args>
void Info(const std::string& fmt, Args... args)
{
    if constexpr (Debugging)
        ::Runic::Log::Core()->info(fmt::runtime(fmt), args...);
}

template <typename... Args>
void Warn(const std::string& fmt, Args... args)
{
    if constexpr (Debugging)
        ::Runic::Log::Core()->warn(fmt::runtime(fmt), args...);
}

template <typename... Args>
void Error(const std::string& fmt, Args... args)
{
    if constexpr (Debugging)
        ::Runic::Log::Core()->error(fmt::runtime(fmt), args...);
}

template <typename... Args>
void Critical(const std::string& fmt, Args... args)
{
    if constexpr (Debugging)
        ::Runic::Log::Core()->critical(fmt::runtime(fmt), args...);
}

template <typename... Args>
constexpr void Assert(bool test, const std::string& fmt, Args... args)
{
    if constexpr (Debugging)
    {
        if (!test)
        {
            Critical("Assertion failed: " + fmt, args...);
            if (!raise(SIGTRAP))
                abort();
        }
    }
}

} // namespace Core

template <typename... Args>
void Trace(const std::string& fmt, Args... args)
{
    if constexpr (Core::Debugging)
        ::Runic::Log::Client()->trace(fmt::runtime(fmt), args...);
}

template <typename... Args>
void Info(const std::string& fmt, Args... args)
{
    if constexpr (Core::Debugging)
        ::Runic::Log::Client()->info(fmt::runtime(fmt), args...);
}

template <typename... Args>
void Warn(const std::string& fmt, Args... args)
{
    if constexpr (Core::Debugging)
        ::Runic::Log::Client()->warn(fmt::runtime(fmt), args...);
}

template <typename... Args>
void Error(const std::string& fmt, Args... args)
{
    if constexpr (Core::Debugging)
        ::Runic::Log::Client()->error(fmt::runtime(fmt), args...);
}

template <typename... Args>
void Critical(const std::string& fmt, Args... args)
{
    if constexpr (Core::Debugging)
        ::Runic::Log::Client()->critical(fmt::runtime(fmt), args...);
}

template <typename... Args>
constexpr void Assert(bool test, const std::string& fmt, Args... args)
{
    if constexpr (Core::Debugging)
    {
        if (!test)
        {
            Critical("Assertion failed: " + fmt, args...);
            if (!raise(SIGTRAP))
                abort();
        }
    }
}

} // namespace Runic
