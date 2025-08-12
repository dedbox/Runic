#pragma once

#ifdef RUNIC_PLATFORM_LINUX

#include "Runic/Application.hpp"

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
int main(int /*argc*/, char** /*argv*/)
{
    Runic::Log::Init();
    RUNIC_CORE_WARN("Initialized log!");
    RUNIC_CORE_INFO("Hello!");

    const auto app = Runic::CreateApplication();
    app->run();
    delete app;
}

#endif // RUNIC_PLATFORM_LINUX
