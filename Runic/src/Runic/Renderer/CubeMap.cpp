#include "Runic/Renderer/CubeMap.hpp"

namespace Runic
{

CubeMap::CubeMap(GraphicsContext* context, RendererId id)
    : _context(context)
    , _id(id)
{
}

std::shared_ptr<CubeMap> CubeMap::Create(
    GraphicsContext* context, const std::vector<SDL_Surface*>& surfaces)
{
    RendererId id = context->createCubeMap(surfaces);

    return std::shared_ptr<CubeMap>(new CubeMap(context, id));
}

CubeMap::~CubeMap()
{
    _context->destroyCubeMap(_id);
}

CubeMap::CubeMap(CubeMap&& other) noexcept
    : _context(std::exchange(other._context, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

CubeMap& CubeMap::operator=(CubeMap&& other) noexcept
{
    if (this != &other)
    {
        _context->destroyCubeMap(_id);
        _context = std::exchange(other._context, nullptr);
        _id      = std::exchange(other._id, 0);
    }
    return *this;
}

void CubeMap::bind() const
{
    _context->bindCubeMap(_id);
}

void CubeMap::unbind() const
{
    _context->unbindCubeMap();
}

} // namespace Runic
