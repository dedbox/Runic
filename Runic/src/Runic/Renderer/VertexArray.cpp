#include "VertexArray.hpp"

namespace Runic
{

namespace
{

uint32_t AttributeTypeComponentCount(AttributeType type)
{
    switch (type)
    {
    case AttributeType::None:
        return 0;
    case AttributeType::Bool:
    case AttributeType::Int:
    case AttributeType::Float:
        return 1;
    case AttributeType::Int2:
    case AttributeType::Float2:
        return 2;
    case AttributeType::Int3:
    case AttributeType::Float3:
        return 3;
    case AttributeType::Int4:
    case AttributeType::Float4:
        return 4;
    case AttributeType::Mat3:
        return 3 * 3;
    case AttributeType::Mat4:
        return 4 * 4;
    }
    Core::Assert(false, "unknown vertex attribute type");
    return 0;
}

static uint32_t AttributeTypeComponentSize(AttributeType type)
{
    switch (type)
    {
    case AttributeType::None:
        return 0;
    case AttributeType::Bool:
        return sizeof(bool);
    case AttributeType::Int:
    case AttributeType::Int2:
    case AttributeType::Int3:
    case AttributeType::Int4:
        return sizeof(int);
    case AttributeType::Float:
    case AttributeType::Float2:
    case AttributeType::Float3:
    case AttributeType::Float4:
    case AttributeType::Mat3:
    case AttributeType::Mat4:
        return sizeof(float);
    }
    Core::Assert(false, "unknown vertex attribute type");
    return 0;
}

static uint32_t AttributeTypeSize(AttributeType type)
{
    return AttributeTypeComponentCount(type) * AttributeTypeComponentSize(type);
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
        _gc->destroyVertexArray(_id);
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
        _gc->destroyVertexArray(_id);
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
    std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<VertexAttribute>& layout)
{
    uint32_t stride = 0;
    for (const auto& attribute : layout)
        stride += AttributeTypeSize(attribute.type);

    uint32_t offset = 0;
    for (const auto&& [index, attribute] : layout | std::ranges::views::enumerate)
    {
        _gc->enableVertexAttribute(index);
        _gc->defineVertexAttributeData(
            index,
            AttributeTypeComponentCount(attribute.type),
            attribute.type,
            attribute.normalize,
            stride,
            reinterpret_cast<const void*>(offset)); // NOLINT
        offset += AttributeTypeSize(attribute.type);
    }

    _vertexBuffers.push_back(std::move(vertexBuffer));
}

void VertexArray::setIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
{
    _indexBuffer = std::move(indexBuffer);
}

void VertexArray::draw() const
{
    _gc->drawIndexed(
        _indexBuffer->getMode(), _indexBuffer->getCount(), _indexBuffer->getType(), nullptr);
}

} // namespace Runic
