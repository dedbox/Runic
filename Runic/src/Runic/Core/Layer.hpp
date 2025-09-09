#pragma once

namespace Runic
{

class Layer
{
public:
    explicit Layer(std::string name = "Layer")
        : _name(std::move(name))
    {
    }

    virtual ~Layer() = default;

    // allow moving
    Layer(Layer&&)            = default;
    Layer& operator=(Layer&&) = default;

    // prevent copying
    Layer(const Layer&)            = delete;
    Layer& operator=(const Layer&) = delete;

    virtual void attach() {}
    virtual void detach() {}
    virtual void update(double deltaTime) {}
    virtual void render() {}

    const std::string& getName() const { return _name; }

private:
    std::string _name;
};

} // namespace Runic
