#pragma once

#include "Runic/Layer.hpp"

namespace Runic
{
class RUNIC_API ImGuiLayer : public Layer
{
public:
    ImGuiLayer();

    ~ImGuiLayer() override = default;

    void onAttach() override;

    void onDetach() override;

    void onUpdate() override;
};
} // Runic
