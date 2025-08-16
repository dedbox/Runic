#pragma once

#include "Runic/Renderer/RendererAPI.hpp"
#include "Runic/Renderer/VertexArray.hpp"

namespace Runic
{
class Renderer
{
public:
    static void beginScene();

    static void endScene();

    static void submit(const std::shared_ptr<VertexArray>& vertexArray);

    static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
};
} // Runic
