#include "pch.hpp"
#include "Runic/Renderer/RenderCommand.hpp"
#include "Runic/Renderer/Renderer.hpp"

namespace Runic
{
void Renderer::beginScene() {}
void Renderer::endScene() {}

void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray)
{
    vertexArray->bind();
    RenderCommand::drawIndexed(vertexArray);
}
} // Runic
