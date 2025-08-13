#pragma once

#include "pch.hpp"
#include "Runic/Window.hpp"

namespace Runic
{
class RUNIC_API Application
{
public:
    Application();

    virtual ~Application() = default;

    void run();

private:
    std::unique_ptr<Window> _window;
    bool _isRunning = true;
};

// Client must define this
extern Application* CreateApplication();
} // Runic
