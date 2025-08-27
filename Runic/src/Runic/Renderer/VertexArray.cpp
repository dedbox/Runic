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
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : _gc(other._gc)
    , _id(other._id)
    , _vertexBuffers(std::move(other._vertexBuffers))
    , _indexBuffer(std::move(other._indexBuffer))
{
    other._id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other)
    {
        if (_id != 0)
        {
            Core::Assert(_id != other._id, "duplicate array id");
            _gc->deleteVertexArray(_id);
        }
        _id            = other._id;
        _vertexBuffers = std::move(other._vertexBuffers);
        _indexBuffer   = std::move(other._indexBuffer);
        other._id      = 0;
    }
    return *this;
}

VertexArray::~VertexArray()
{
    if (_id != 0)
    {
        _gc->deleteVertexArray(_id);
        _id = 0;
    }
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
    bind();
    vertexBuffer->bind();

    uint32_t stride = 0;
    for (const auto& element : layout)
        stride += ElementTypeSize(element.type);

    uint32_t offset = 0;
    for (const auto&& [index, element] : layout | std::ranges::views::enumerate)
    {
        _gc->enableVertexAttribute(index);
        _gc->defineVertexAttributeData(
            index, ElementTypeComponentCount(element.type), element.type, element.normalized,
            stride,
            reinterpret_cast<const void*>(offset)); // NOLINT
        offset += ElementTypeSize(element.type);
    }

    _vertexBuffers.push_back(std::move(vertexBuffer));
}

void VertexArray::setIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
{
    bind();
    indexBuffer->bind();
    _indexBuffer = std::move(indexBuffer);
}

} // namespace Runic
