#pragma once

#include "glm/ext/matrix_float4x4.hpp"

#include "Runic/Renderer/Camera.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Mesh.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"
#include "Runic/Renderer/Texture.hpp"

namespace Runic
{

class RenderObject
{
protected:
    GraphicsContext* _context;

    explicit RenderObject(GraphicsContext* context);

public:
    std::unique_ptr<Mesh> mesh;
    std::shared_ptr<ShaderProgram> shader;

    glm::vec3 position = {0.0F, 0.0F, 0.0F};
    glm::vec3 scale    = {1.0F, 1.0F, 1.0F};

    glm::vec3 rotationAxis = {0.0F, 1.0F, 0.0F}; /**< rotation axis */
    float rotation         = 0.0F;               /**< rotation angle (degrees) */

    static std::unique_ptr<RenderObject> Create(GraphicsContext* context);

    void createMesh(DrawMode mode);

    TextureId addTexture(const std::string& name, const std::string& path);

    glm::mat4 modelMatrix() const;

    /** Draws the mesh with the shader. */
    void draw();

    /** Draws the mesh with the shader, as viewed from a camera.
     *
     * Assumes the shader follows our MVP uniform convention: model, view, projection.
     */
    void draw(const Camera& camera);

private:
    std::map<std::string, TextureId> _textures;
};

} // namespace Runic
