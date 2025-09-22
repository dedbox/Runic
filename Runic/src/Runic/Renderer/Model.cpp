#include "Runic/Renderer/Model.hpp"

#include "Runic/Renderer/TextureManager.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/types.h"
#include "assimp/vector3.h"

namespace Runic
{

Model::Model(GraphicsContext* context, std::string&& directory)
    : _context(context)
    , _directory(std::move(directory))
{
}

std::unique_ptr<Model> Model::Create(GraphicsContext* context, const std::string& path)
{
    const std::filesystem::path currentDir = std::filesystem::current_path();
    std::println("Current working directory: {}", currentDir.c_str());

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Core::Error("Assimp: {}", importer.GetErrorString());
        return nullptr;
    }

    auto model = std::unique_ptr<Model>(new Model(context, path.substr(0, path.find_last_of('/'))));
    model->processNode(scene->mRootNode, scene);

    return model;
}

void Model::addCubeMap(const std::string& name, std::shared_ptr<CubeMap> cubeMap)
{
    for (auto& mesh : _meshes)
        mesh->addCubeMap(name, cubeMap);
}

void Model::draw(const ShaderProgram& shader, const Camera& camera)
{
    shader.bind();
    shader.setUniform("model", glm::mat4(1.0F));
    shader.setUniform("view", camera.viewMatrix());
    shader.setUniform("projection", camera.projectionMatrix());
    shader.unbind();

    for (const auto& mesh : _meshes)
        mesh->draw(shader);
}

void Model::processNode(aiNode* node, const aiScene* scene) // NOLINT
{
    const auto scene_meshes = std::span<aiMesh*>(scene->mMeshes, scene->mNumMeshes);

    for (const auto i : std::span<unsigned int>(node->mMeshes, node->mNumMeshes))
        _meshes.push_back(processMesh(scene_meshes[i], scene));

    for (const auto child : std::span<aiNode*>(node->mChildren, node->mNumChildren))
        processNode(child, scene);
}

std::unique_ptr<Mesh> Model::processMesh(aiMesh* raw_mesh, const aiScene* scene)
{
    if (!raw_mesh->HasNormals())
        throw std::runtime_error("Imported mesh does not have normals");

    if (!raw_mesh->HasTextureCoords(0))
        throw std::runtime_error("Imported mesh does not have texture coordinates");

    if (raw_mesh->mMaterialIndex < 0)
        throw std::runtime_error("Imported mesh does not have a material");

    auto mesh = Mesh::Create(_context, DrawMode::Triangles);

    std::vector<float> vertices;

    for (const auto&& [vertex, normal, texCoord] : std::ranges::views::zip(
             std::span<aiVector3D>(raw_mesh->mVertices, raw_mesh->mNumVertices),
             std::span<aiVector3D>(raw_mesh->mNormals, raw_mesh->mNumVertices),
             std::span<aiVector3D>(raw_mesh->mTextureCoords[0], raw_mesh->mNumVertices)))
    {
        // position
        vertices.push_back(vertex.x);
        vertices.push_back(vertex.y);
        vertices.push_back(vertex.z);
        // normal
        vertices.push_back(normal.x);
        vertices.push_back(normal.y);
        vertices.push_back(normal.z);
        // texture coordinates
        vertices.push_back(texCoord.x);
        vertices.push_back(texCoord.y);
    }

    mesh->addVertexBuffer(
        VertexBuffer::Create(_context, vertices, BufferUsage::Static),
        {
            {.type = AttributeType::Float3, .normalize = false},
            {.type = AttributeType::Float3, .normalize = false},
            {.type = AttributeType::Float2, .normalize = false},
        });

    std::vector<uint32_t> indices;

    for (const auto& face : std::span<aiFace>(raw_mesh->mFaces, raw_mesh->mNumFaces))
        for (const auto i : std::span<uint32_t>(face.mIndices, face.mNumIndices))
            indices.push_back(i);

    mesh->setIndices(indices, IndexType::Int, BufferUsage::Static);

    const auto scene_materials = std::span<aiMaterial*>(scene->mMaterials, scene->mNumMaterials);
    aiMaterial* material       = scene_materials[raw_mesh->mMaterialIndex];

    loadMaterialTextures(*mesh, material, aiTextureType_DIFFUSE, "texture_diffuse");
    loadMaterialTextures(*mesh, material, aiTextureType_SPECULAR, "texture_specular");

    return mesh;
}

void Model::loadMaterialTextures(
    Mesh& mesh, aiMaterial* mat, aiTextureType type, const std::string& typeName)
{
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        mesh.addTexture(
            std::format("{}{}", typeName, i + 1),
            TextureManager::Find(_context, std::format("{}/{}", _directory, str.C_Str())));
    }
}

} // namespace Runic
