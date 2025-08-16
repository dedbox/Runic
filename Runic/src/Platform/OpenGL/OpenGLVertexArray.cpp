#include "pch.hpp"
#include "OpenGLVertexArray.hpp"

#include "glad/gl.h"

namespace Runic
{
static GLenum shaderDataTypeToOpenGLBaseType(const ShaderDataType type)
{
    switch (type) {
        case ShaderDataType::None:
            return GL_NONE;
        case ShaderDataType::Bool:
            return GL_BOOL;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GL_FLOAT;
    }
    RUNIC_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return GL_NONE;
}

OpenGLVertexArray::OpenGLVertexArray()
{
    glCreateVertexArrays(1, &_rendererID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &_rendererID);
}

void OpenGLVertexArray::bind() const
{
    glBindVertexArray(_rendererID);
}

void OpenGLVertexArray::unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    RUNIC_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex buffer has no layout!");

    glBindVertexArray(_rendererID);
    vertexBuffer->bind();

    for (const auto&& [index, element] : vertexBuffer->getLayout().getElements() | std::ranges::views::enumerate) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            element.getComponentCount(),
            shaderDataTypeToOpenGLBaseType(element.type),
            element.normalized ? GL_TRUE : GL_FALSE,
            vertexBuffer->getLayout().getStride(),
            reinterpret_cast<const void*>(element.offset));
    }

    _vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    glBindVertexArray(_rendererID);
    indexBuffer->bind();
    _indexBuffer = indexBuffer;
}
} // Runic
