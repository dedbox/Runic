#include <Runic.hpp>

class SandboxApplication : public Runic::Application
{
};

Runic::Application* Runic::CreateApplication() {
  return new SandboxApplication;
}
