#pragma once

#ifdef RUNIC_PLATFORM_LINUX

#include "Application.hpp"

// ReSharper disable once CppNonInlineFunctionDefinitionInHeaderFile
int main(int /*argc*/, char** /*argv*/)
{
    const auto app = Runic::CreateApplication();
    app->run();
    delete app;
}

#endif // RUNIC_PLATFORM_LINUX
