#pragma once

#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"

#include "Runic/Renderer/Camera.hpp"
#include "Runic/Renderer/GraphicsContext.hpp"
#include "Runic/Renderer/Mesh.hpp"
#include "Runic/Renderer/ShaderProgram.hpp"

namespace Runic
{

class Model
{
private:
    GraphicsContext* _context;

    Model(GraphicsContext* context, std::string&& directory);

public:
    static std::unique_ptr<Model> Create(GraphicsContext* context, const std::string& path);

    void draw(const ShaderProgram& shaderProgram, const Camera& camera);

private:
    std::vector<std::unique_ptr<Mesh>> _meshes;
    std::string _directory;

    void processNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

    void loadMaterialTextures(
        Mesh& mesh, aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

} // namespace Runic
