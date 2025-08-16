#pragma once

#include "pch.hpp"

namespace Runic
{
enum class ShaderDataType
{
    None = 0,
    Bool,
    Int, Int2, Int3, Int4,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
};

static uint32_t shaderDataTypeComponentSize(const ShaderDataType type)
{
    switch (type) {
        case ShaderDataType::None:
            return 0;
        case ShaderDataType::Bool:
            return sizeof(bool);
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return sizeof(int);
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return sizeof(float);
    }

    RUNIC_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

static uint32_t shaderDataTypeComponentCount(const ShaderDataType type)
{
    switch (type) {
        case ShaderDataType::None: return 0;
        case ShaderDataType::Bool: return 1;
        case ShaderDataType::Int: return 1;
        case ShaderDataType::Int2: return 2;
        case ShaderDataType::Int3: return 3;
        case ShaderDataType::Int4: return 4;
        case ShaderDataType::Float: return 1;
        case ShaderDataType::Float2: return 2;
        case ShaderDataType::Float3: return 3;
        case ShaderDataType::Float4: return 4;
        case ShaderDataType::Mat3: return 3 * 3;
        case ShaderDataType::Mat4: return 4 * 4;
    }

    RUNIC_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

static uint32_t shaderDataTypeSize(const ShaderDataType type)
{
    return shaderDataTypeComponentSize(type) * shaderDataTypeComponentCount(type);
}

struct BufferElement
{
    std::string name{};
    ShaderDataType type{};
    uint32_t size{};
    uint32_t offset{};
    bool normalized{};

    BufferElement(const ShaderDataType type, const std::string& name, const bool normalized = false)
        : name(name), type(type), size(shaderDataTypeSize(type)), normalized(normalized) {}

    uint32_t getComponentCount() const { return shaderDataTypeComponentCount(type); }
};

class BufferLayout
{
public:
    BufferLayout() = default;

    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : _elements(elements)
    {
        calculateOffsetsAndStride();
    }

    uint32_t getStride() const { return _stride; }

    const std::vector<BufferElement>& getElements() const { return _elements; }

private:
    std::vector<BufferElement> _elements;
    uint32_t _stride{0};

    void calculateOffsetsAndStride()
    {
        _stride = 0;
        for (auto& element : _elements) {
            element.offset = _stride;
            _stride += element.size;
        }
    }
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;

    virtual void unbind() const = 0;

    virtual const BufferLayout& getLayout() const = 0;

    virtual void setLayout(BufferLayout layout) = 0;

    static VertexBuffer* create(const float* vertices, uint32_t size);
};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const = 0;

    virtual void unbind() const = 0;

    virtual uint32_t getCount() const = 0;

    static IndexBuffer* create(const uint32_t* indices, uint32_t count);
};
}
