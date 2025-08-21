#include "Runic/Core/LayerManager.hpp"
#include <memory>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

namespace
{

class TestLayer : public Runic::Layer
{
public:
    explicit TestLayer(int n)
        : Runic::Layer(std::to_string(n))
    {
    }
};

std::string flatten_layers(const Runic::LayerManager& layers)
{
    return layers |
           std::ranges::views::transform([](const auto& layer) { return layer->getName(); }) |
           std::ranges::views::join_with(' ') | std::ranges::to<std::string>();
}

} // namespace

// NOLINTBEGIN(misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while,
// cppcorebuidelines-avoid-magic-numbers)

TEST_CASE("empty LayerManager can grow")
{
    Runic::LayerManager layers;

    SUBCASE("push_front")
    {
        layers.push_front(std::make_unique<TestLayer>(1));
        layers.push_front(std::make_unique<TestLayer>(2));
        CHECK(flatten_layers(layers) == "2 1");
    }

    SUBCASE("pushGui_front")
    {
        layers.pushGui_front(std::make_unique<TestLayer>(1));
        layers.pushGui_front(std::make_unique<TestLayer>(2));
        CHECK(flatten_layers(layers) == "2 1");
    }

    SUBCASE("push_back")
    {
        layers.push_back(std::make_unique<TestLayer>(1));
        layers.push_back(std::make_unique<TestLayer>(2));
        CHECK(flatten_layers(layers) == "1 2");
    }

    SUBCASE("pushGui_back")
    {
        layers.pushGui_back(std::make_unique<TestLayer>(8));
        layers.pushGui_back(std::make_unique<TestLayer>(9));
        CHECK(flatten_layers(layers) == "8 9");
    }

    SUBCASE("interleaved pushes")
    {
        layers.pushGui_back(std::make_unique<TestLayer>(1));
        layers.push_back(std::make_unique<TestLayer>(2));
        layers.pushGui_front(std::make_unique<TestLayer>(3));
        layers.push_front(std::make_unique<TestLayer>(4));
        CHECK(flatten_layers(layers) == "4 2 3 1");
    }
}

TEST_CASE("populated ListManager can shrink")
{
    Runic::LayerManager layers;

    layers.push_back(std::make_unique<TestLayer>(1));
    layers.push_back(std::make_unique<TestLayer>(2));
    layers.push_back(std::make_unique<TestLayer>(3));
    layers.push_back(std::make_unique<TestLayer>(4));
    layers.pushGui_back(std::make_unique<TestLayer>(6));
    layers.pushGui_back(std::make_unique<TestLayer>(7));
    layers.pushGui_back(std::make_unique<TestLayer>(8));
    layers.pushGui_back(std::make_unique<TestLayer>(9));

    CHECK(flatten_layers(layers) == "1 2 3 4 6 7 8 9");

    SUBCASE("pop_front")
    {
        layers.pop_front();
        CHECK(flatten_layers(layers) == "2 3 4 6 7 8 9");

        layers.pop_front();
        CHECK(flatten_layers(layers) == "3 4 6 7 8 9");
    }

    SUBCASE("popGui_front")
    {
        layers.popGui_front();
        CHECK(flatten_layers(layers) == "1 2 3 4 7 8 9");

        layers.popGui_front();
        CHECK(flatten_layers(layers) == "1 2 3 4 8 9");
    }

    SUBCASE("pop_back")
    {
        layers.pop_back();
        CHECK(flatten_layers(layers) == "1 2 3 6 7 8 9");

        layers.pop_back();
        CHECK(flatten_layers(layers) == "1 2 6 7 8 9");
    }

    SUBCASE("popGui_back")
    {
        layers.popGui_back();
        CHECK(flatten_layers(layers) == "1 2 3 4 6 7 8");

        layers.popGui_back();
        CHECK(flatten_layers(layers) == "1 2 3 4 6 7");
    }

    SUBCASE("interleaved pops")
    {
        layers.popGui_front();
        layers.pop_back();
        CHECK(flatten_layers(layers) == "1 2 3 7 8 9");

        layers.pop_front();
        layers.popGui_back();
        CHECK(flatten_layers(layers) == "2 3 7 8");
    }
}

// NOLINTEND(misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while,
// cppcorebuidelines-avoid-magic-numbers)
