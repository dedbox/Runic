#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Runic/Renderer/Camera.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Material.hpp"
#include "Runic/Renderer/Mesh.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"
#include "Runic/Renderer/Texture.hpp"
#include "Runic/Renderer/TextureManager.hpp"

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

    glm::vec3 rotationAxis = {0.0F, 1.0F, 0.0F}; /**< rotation axis */
    float rotation         = 0.0F;               /**< rotation angle (degrees) */

    static std::unique_ptr<RenderObject> Create(GraphicsContext* context)
    {
        return std::unique_ptr<RenderObject>(new RenderObject(context));
    }

    void createMesh(DrawMode mode) { mesh = Mesh<Layouts...>::Create(_context, mode); }

    TextureId addTexture(const std::string& path)
    {
        if (_textures.contains(path))
            return _textures[path];

        mesh->addTexture(TextureManager::Find(_context, path));

        TextureId id    = _textures.size();
        _textures[path] = id;

        return id;
    }

    void setMaterial(const std::string& name, Material material)
    {
        std::visit(
            [&](auto&& material) {
                // using T = std::decay_t<decltype(material)>;
                // if constexpr (std::is_same_v<T, PhongMaterial>)

                shader->bind();
                shader->setUniform(std::format("{}.diffuse", name), material.diffuse);
                shader->setUniform(std::format("{}.specular", name), material.specular);
                shader->setUniform(std::format("{}.shininess", name), material.shininess);
                shader->unbind();
            },
            material);

        _material = material;
    }

    void unsetMaterial() { _material = std::nullopt; };

    std::optional<Material> material() const { return _material; }

    glm::mat4 modelMatrix() const
    {
        glm::mat4 model(1.0F);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation), rotationAxis);
        model = glm::scale(model, scale);
        return model;
    }

    /** Draws the mesh with the shader. */
    void draw() { mesh->draw(*shader); }

    /** Draws the mesh with the shader, as viewed from a camera.
     *
     * Assumes the shader follows our MVP uniform convention: model, view, projection.
     */
    void draw(const Camera& camera)
    {
        shader->bind();
        shader->setUniform("model", modelMatrix());
        shader->setUniform("view", camera.viewMatrix());
        shader->setUniform("projection", camera.projectionMatrix());
        shader->unbind();

        mesh->draw(*shader);
    }

private:
    std::map<std::string, TextureId> _textures;
    std::optional<Material> _material;
};

} // namespace Runic
