#include "Runic/Renderer/VertexArray.hpp"

namespace Runic
{

namespace
{

size_t AttributeComponentCount(const VertexAttribute& attr)
{
    switch (attr.type)
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
        return 9;
    case AttributeType::Mat4:
        return 16;
    }
    Core::Assert(false, "Unknown vertex attribute type");
    return 0;
}

size_t AttributeComponentSize(const VertexAttribute& attr)
{
    switch (attr.type)
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
    Core::Assert(false, "Unknown vertex attribute type");
    return 0;
}

size_t AttributeSize(const VertexAttribute& attr)
{
    return AttributeComponentCount(attr) * AttributeComponentSize(attr);
}

} // namespace

VertexArray::VertexArray(GraphicsContext* context, RendererId id, DrawMode mode)
    : _context(context)
    , _id(id)
    , _mode(mode)
{
    bind();
}

std::unique_ptr<VertexArray> VertexArray::Create(GraphicsContext* context, DrawMode mode)
{
    RendererId id = context->createVertexArray();
    return std::unique_ptr<VertexArray>(new VertexArray(context, id, mode));
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : _context(std::exchange(other._context, nullptr))
    , _id(std::exchange(other._id, 0))
    , _mode(other._mode)
{
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other)
    {
        _context->destroyVertexArray(_id);
        _context = std::exchange(other._context, nullptr);
        _id      = std::exchange(other._id, 0);
        _mode    = other._mode;
    }
    return *this;
}

VertexArray::~VertexArray()
{
    if (_id != 0)
    {
        _context->destroyVertexArray(_id);
        _id = 0;
    }
}

void VertexArray::bind() const
{
    _context->bindVertexArray(_id);
}

void VertexArray::unbind() const
{
    _context->unbindVertexArray();
}

void VertexArray::addVertexBuffer(
    std::unique_ptr<VertexBuffer> vertexBuffer, const std::vector<VertexAttribute>& layout)
{
    size_t stride = 0;
    for (const auto& attr : layout)
        stride += AttributeSize(attr);

    _count += vertexBuffer->getSize() / stride;

    size_t offset = 0;
    for (const auto& [i, attr] : std::ranges::views::enumerate(layout))
    {
        _context->enableVertexAttribute(i);
        _context->defineVertexAttributeData(
            i,
            AttributeComponentCount(attr),
            attr.type,
            attr.normalize,
            stride,
            reinterpret_cast<const void*>(offset)); // NOLINT
        offset += AttributeSize(attr);
    }

    _vertexBuffers.push_back(std::move(vertexBuffer));
}

void VertexArray::setIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
{
    _indexBuffer = std::move(indexBuffer);
}

void VertexArray::draw() const
{
    if (_indexBuffer)
        _context->drawIndexed(_mode, _indexBuffer->getCount(), _indexBuffer->getType(), nullptr);
    else
    {
        _context->drawVertices(_mode, 0, _count);
    }
}

} // namespace Runic
