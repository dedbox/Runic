#pragma once

#include "Runic/Renderer/Camera.hpp"
#include "Runic/Renderer/RendererAPI.hpp"
#include "Runic/Renderer/Shader.hpp"
#include "Runic/Renderer/VertexArray.hpp"

namespace Runic
{
class Renderer
{
public:
    static void beginScene(const OrthographicCamera& camera);

    static void endScene();

    static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

private:
    struct SceneData
    {
        glm::mat4 viewProjectionMatrix;
    };

    static SceneData* s_SceneData;
};
} // Runic
