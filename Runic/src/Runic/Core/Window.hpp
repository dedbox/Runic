#pragma once

namespace Runic
{
struct WindowData
{
    std::string title{"Runic Engine"};
    uint32_t width{1280};
    uint32_t height{720};
};

class Window
{
  public:
    explicit Window(const WindowData& data);

    const WindowData& getData() const;

  private:
    WindowData _data;
};
} // namespace Runic
