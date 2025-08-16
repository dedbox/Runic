#include "Runic/Renderer/RenderCommand.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Runic
{
RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}
