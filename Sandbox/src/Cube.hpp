#pragma once

#include <Runic.hpp>

using Pos3  = Runic::Attribute<float, 3>;
using Norm3 = Runic::Attribute<float, 3>;

using CubeLayout = Runic::Layout<Pos3, Norm3>;

class Cube : public Runic::RenderObject<CubeLayout>
{
private:
    explicit Cube(Runic::GraphicsContext* context)
        : Runic::RenderObject<CubeLayout>(context)
    {
    }

public:
    static std::unique_ptr<Cube> Create(
        Runic::GraphicsContext* context, std::shared_ptr<Runic::ShaderProgram<CubeLayout>>&& shader)
    {
        auto cube = std::unique_ptr<Cube>(new Cube(context));

        cube->createMesh(Runic::DrawMode::Triangles);
        cube->mesh->setVertexBuffers(
            std::make_tuple(
                Runic::VertexBuffer<CubeLayout>::Create(
                    context,
                    Runic::VertexData<CubeLayout>(cube->_vertices),
                    Runic::BufferUsage::Static)));

        cube->shader = std::move(shader);

        return cube;
    }

private:
    // clang-format off
    std::vector<float> _vertices = {
        // a_Position      | a_Normal
        -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F,
         0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
         0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
         0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
        -0.5F,  0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 
        -0.5F, -0.5F, -0.5F,  0.0F,  0.0F, -1.0F, 

        -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
         0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
         0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
         0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
        -0.5F,  0.5F,  0.5F,  0.0F,  0.0F, 1.0F,
        -0.5F, -0.5F,  0.5F,  0.0F,  0.0F, 1.0F,

        -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,
        -0.5F,  0.5F, -0.5F, -1.0F,  0.0F,  0.0F,
        -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,
        -0.5F, -0.5F, -0.5F, -1.0F,  0.0F,  0.0F,
        -0.5F, -0.5F,  0.5F, -1.0F,  0.0F,  0.0F,
        -0.5F,  0.5F,  0.5F, -1.0F,  0.0F,  0.0F,

         0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,
         0.5F,  0.5F, -0.5F,  1.0F,  0.0F,  0.0F,
         0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,
         0.5F, -0.5F, -0.5F,  1.0F,  0.0F,  0.0F,
         0.5F, -0.5F,  0.5F,  1.0F,  0.0F,  0.0F,
         0.5F,  0.5F,  0.5F,  1.0F,  0.0F,  0.0F,

        -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,
         0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,
         0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,
         0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,
        -0.5F, -0.5F,  0.5F,  0.0F, -1.0F,  0.0F,
        -0.5F, -0.5F, -0.5F,  0.0F, -1.0F,  0.0F,

        -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,
         0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F,
         0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,
         0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,
        -0.5F,  0.5F,  0.5F,  0.0F,  1.0F,  0.0F,
        -0.5F,  0.5F, -0.5F,  0.0F,  1.0F,  0.0F
    };
    // clang-format on
};
