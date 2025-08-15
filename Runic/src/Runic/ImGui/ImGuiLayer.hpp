#pragma once

#include "Runic/Layer.hpp"

namespace Runic
{
class RUNIC_API ImGuiLayer final : public Layer
{
public:
    ImGuiLayer();

    ~ImGuiLayer() override = default;

    void onAttach() override;

    void onDetach() override;

    void onImGuiRender() override;

    static void begin();

    static void end();
};
} // Runic
