#include "VertexArray.hpp"

namespace Runic
{

namespace
{

uint32_t ElementTypeComponentCount(ElementType type)
{
    switch (type)
    {
    case ElementType::None: return 0;
    case ElementType::Bool:
    case ElementType::Int:
    case ElementType::Float: return 1;
    case ElementType::Int2:
    case ElementType::Float2: return 2;
    case ElementType::Int3:
    case ElementType::Float3: return 3;
    case ElementType::Int4:
    case ElementType::Float4: return 4;
    case ElementType::Mat3: return 3 * 3;
    case ElementType::Mat4: return 4 * 4;
    }
    Core::Assert(false, "unknown element type");
    return 0;
}

static uint32_t ElementTypeComponentSize(ElementType type)
{
    switch (type)
    {
    case ElementType::None: return 0;
    case ElementType::Bool: return sizeof(bool);
    case ElementType::Int:
    case ElementType::Int2:
    case ElementType::Int3:
    case ElementType::Int4: return sizeof(int);
    case ElementType::Float:
    case ElementType::Float2:
    case ElementType::Float3:
    case ElementType::Float4:
    case ElementType::Mat3:
    case ElementType::Mat4: return sizeof(float);
    }
    Core::Assert(false, "unknown element type");
    return 0;
}

static uint32_t ElementTypeSize(ElementType type)
{
    return ElementTypeComponentCount(type) * ElementTypeComponentSize(type);
}

} // namespace

VertexArray::VertexArray(GraphicsContext* gc, RendererId id)
    : _gc(gc)
    , _id(id)
{
    bind();
}

VertexArray::~VertexArray()
{
    if (_id != 0)
    {
        _gc->deleteVertexArray(_id);
        _id = 0;
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : _gc(std::exchange(other._gc, nullptr))
    , _id(std::exchange(other._id, 0))
{
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other)
    {
        _gc->deleteVertexArray(_id);
        _gc = std::exchange(other._gc, nullptr);
        _id = std::exchange(other._id, 0);
    }
    return *this;
}

void VertexArray::bind() const
{
    _gc->bindVertexArray(_id);
}

void VertexArray::unbind() const
{
    _gc->unbindVertexArray();
}

void VertexArray::addVertexBuffer(
    std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<LayoutElement>& layout)
{
    uint32_t stride = 0;
    for (const auto& element : layout)
        stride += ElementTypeSize(element.type);

    uint32_t offset = 0;
    for (const auto&& [index, element] : layout | std::ranges::views::enumerate)
    {
        _gc->enableVertexAttribute(index);
        _gc->defineVertexAttributeData(
            index, ElementTypeComponentCount(element.type), element.type, element.normalize, stride,
            reinterpret_cast<const void*>(offset)); // NOLINT
        offset += ElementTypeSize(element.type);
    }

    _vertexBuffers.push_back(std::move(vertexBuffer));
}

void VertexArray::setIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
{
    _indexBuffer = std::move(indexBuffer);
}

void VertexArray::draw() const
{
    _gc->drawElements(
        _indexBuffer->getMode(), _indexBuffer->getCount(), _indexBuffer->getType(), nullptr);
}

} // namespace Runic
