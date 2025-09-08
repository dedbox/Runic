#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Runic/Renderer/Camera.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Mesh.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"

namespace Runic
{

template <typename... Layouts>
class RenderObject
{
protected:
    GraphicsContext* _context;

    explicit RenderObject(GraphicsContext* context)
        : _context(context)
    {
    }

public:
    std::unique_ptr<Mesh<Layouts...>> mesh;
    std::shared_ptr<ShaderProgram<Layouts...>> shader;

    glm::vec3 position = {0.0F, 0.0F, 0.0F};
    glm::vec3 scale    = {1.0F, 1.0F, 1.0F};

    static std::unique_ptr<RenderObject> Create(GraphicsContext* context)
    {
        return std::unique_ptr<RenderObject>(new RenderObject(context));
    }

    void createMesh(DrawMode mode) { mesh = Mesh<Layouts...>::Create(_context, mode); }

    glm::mat4 modelMatrix() const
    {
        glm::mat4 model(1.0F);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        return model;
    }

    /** Draws the mesh with the shader. */
    void draw() { mesh->draw(*shader); }

    /** Draws the mesh with the shader, as viewed from a camera.
     *
     * Assumes the shader follows our MVP uniform convention: u_Model, u_View, u_Projection.
     */
    void draw(const Camera& camera)
    {
        shader->bind();
        shader->setUniform("u_Model", modelMatrix());
        shader->setUniform("u_View", camera.viewMatrix());
        shader->setUniform("u_Projection", camera.projectionMatrix());
        shader->unbind();

        mesh->draw(*shader);
    }
};

} // namespace Runic
