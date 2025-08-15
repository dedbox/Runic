#pragma once

#include "pch.hpp"

namespace Runic
{
class Shader
{
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);

    ~Shader();

    void bind() const;

    static void unbind();

private:
    uint32_t _rendererID;
};
}
