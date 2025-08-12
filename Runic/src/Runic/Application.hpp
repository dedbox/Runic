#pragma once

#include "Runic/Core.hpp"

namespace Runic
{
class Application
{
public:
    Application() = default;

    virtual ~Application() = default;

    void RUNIC_API run();
};

// Client must define this
extern Application* CreateApplication();
} // Runic
