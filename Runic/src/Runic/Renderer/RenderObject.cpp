#include "Runic/Renderer/RenderObject.hpp"

#include "glm/ext/matrix_transform.hpp"

#include "Runic/Renderer/TextureManager.hpp"

namespace Runic
{

RenderObject::RenderObject(GraphicsContext* context)
    : _context(context)
{
}

std::unique_ptr<RenderObject> RenderObject::Create(GraphicsContext* context)
{
    return std::unique_ptr<RenderObject>(new RenderObject(context));
}

void RenderObject::createMesh(DrawMode mode)
{
    mesh = Mesh::Create(_context, mode);
}

TextureId RenderObject::addTexture(const std::string& name, const std::string& path)
{
    if (_textures.contains(path))
        return _textures[path];

    mesh->addTexture(name, TextureManager::Find(_context, path));

    auto id = static_cast<TextureId>(_textures.size());

    _textures[path] = id;

    return id;
}

glm::mat4 RenderObject::modelMatrix() const
{
    glm::mat4 model(1.0F);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation), rotationAxis);
    model = glm::scale(model, scale);
    return model;
}

void RenderObject::draw()
{
    mesh->draw(*shader);
}

void RenderObject::draw(const Camera& camera)
{
    shader->bind();
    shader->setUniform("model", modelMatrix());
    shader->setUniform("view", camera.viewMatrix());
    shader->setUniform("projection", camera.projectionMatrix());
    shader->unbind();

    mesh->draw(*shader);
}

} // namespace Runic
