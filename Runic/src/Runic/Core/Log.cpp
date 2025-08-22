#include "Log.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace Runic
{

std::shared_ptr<spdlog::logger> Log::CoreLogger;
std::shared_ptr<spdlog::logger> Log::ClientLogger;

void Log::Init(const std::string& clientName)
{
    spdlog::set_pattern("%^[%T] %n: %v%$");

    CoreLogger = spdlog::stdout_color_mt("RUNIC");
    CoreLogger->set_level(spdlog::level::trace);

    ClientLogger = spdlog::stdout_color_mt(clientName);
    ClientLogger->set_level(spdlog::level::trace);
}

} // namespace Runic
