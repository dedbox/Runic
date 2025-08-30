#include "Runic/Core/LayerManager.hpp"
#include "Runic/Util/AssocList.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// LayerManager ----------------------------------------------------------------

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

// NOLINTBEGIN(misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)

TEST_CASE("LayerManager")
{
    SUBCASE("Empty LayerManager can grow")
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

    SUBCASE("Populated LayerManager can shrink")
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
}

// AssocList -------------------------------------------------------------------

namespace
{

std::string flatten_alist(const Runic::AssocList<uint32_t, std::string>& alist)
{
    return alist.data() | std::ranges::views::transform([](const auto& pair) {
               return std::format("{}:{}", std::to_string(pair.first), *pair.second);
           }) |
           std::ranges::views::join_with(' ') | std::ranges::to<std::string>();
}

} // namespace

TEST_CASE("AssocList")
{
    Runic::AssocList<uint32_t, std::string> alist;

    SUBCASE("emplace")
    {
        alist.emplace(6, "A");
        alist.emplace(3, "B");
        alist.emplace(1, "C");
        alist.emplace(2, "D");
        alist.emplace(5, "E");
        alist.emplace(4, "F");
        CHECK(flatten_alist(alist) == "1:C 2:D 3:B 4:F 5:E 6:A");

        SUBCASE("remove")
        {
            alist.remove(5);
            CHECK(flatten_alist(alist) == "1:C 2:D 3:B 4:F 6:A");

            alist.remove(2);
            CHECK(flatten_alist(alist) == "1:C 3:B 4:F 6:A");

            alist.remove(6);
            CHECK(flatten_alist(alist) == "1:C 3:B 4:F");

            alist.remove(1);
            CHECK(flatten_alist(alist) == "3:B 4:F");

            alist.remove(3);
            alist.remove(4);
            CHECK(alist.empty());
        }

        SUBCASE("emplace struct")
        {
            struct TestStruct
            {
                int a, b, c;
            };

            Runic::AssocList<uint32_t, TestStruct> alist2;

            alist2.emplace(3, 9, 8, 7);
            auto value = alist2.find(3).value().get();

            CHECK(value.a == 9);
            CHECK(value.b == 8);
            CHECK(value.c == 7);
        }
    }

    SUBCASE("add")
    {
        auto msg = std::make_unique<std::string>("hello");
        alist.add(1, std::move(msg));
        CHECK(alist.find(1).value().get() == "hello");
    }
}

// NOLINTEND(misc-use-anonymous-namespace, cppcoreguidelines-avoid-do-while)
