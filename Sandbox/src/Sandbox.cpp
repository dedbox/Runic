#include <Runic.hpp>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"

#include "MainLayerBase.hpp"

// MainLayer ---------------------------------------------------------------------------------------

class MainLayer : public MainLayerBase
{
private:
    Runic::Window* _window;
    Runic::GraphicsContext* _context;

public:
    MainLayer(Runic::Window* window, Runic::GraphicsContext* context)
        : MainLayerBase(window, context)
    {
        _redShader   = Runic::ShaderManager::Find(_context, "UniformBuffer", "UniformBufferRed");
        _greenShader = Runic::ShaderManager::Find(_context, "UniformBuffer", "UniformBufferGreen");
        _blueShader  = Runic::ShaderManager::Find(_context, "UniformBuffer", "UniformBufferBlue");
        _yellowShader =
            Runic::ShaderManager::Find(_context, "UniformBuffer", "UniformBufferYellow");

        _cube = Runic::Graphics::Cube::Create(_context);
    }

    void update(double deltaTime) override
    {
        MainLayerBase::update(deltaTime);

        // TODO update view matrix in uniform buffer
    }

    void render() override
    {
        _context->setClearColor(Runic::Color::Gray1);
        _context->clear();

        _redShader->bind();
        auto model = glm::translate(glm::mat4(1.0F), glm::vec3(-0.75F, 0.75F, 0.0F));
        _redShader->setUniform("model", model);
        _cube->draw(*_redShader);

        _greenShader->bind();
        model = glm::translate(glm::mat4(1.0F), glm::vec3(0.75F, 0.75F, 0.0F));
        _greenShader->setUniform("model", model);
        _cube->draw(*_greenShader);

        _blueShader->bind();
        model = glm::translate(glm::mat4(1.0F), glm::vec3(-0.75F, -0.75F, 0.0F));
        _blueShader->setUniform("model", model);
        _cube->draw(*_blueShader);

        _yellowShader->bind();
        model = glm::translate(glm::mat4(1.0F), glm::vec3(0.75F, -0.75F, 0.0F));
        _yellowShader->setUniform("model", model);
        _cube->draw(*_yellowShader);
    }

private:
    std::unique_ptr<Runic::Graphics::Cube> _cube;
    std::shared_ptr<Runic::ShaderProgram> _redShader;
    std::shared_ptr<Runic::ShaderProgram> _greenShader;
    std::shared_ptr<Runic::ShaderProgram> _blueShader;
    std::shared_ptr<Runic::ShaderProgram> _yellowShader;
    Runic::Camera _camera;
};

// Sandbox -----------------------------------------------------------------------------------------

class Sandbox : public Runic::Application
{
public:
    Sandbox()
        : Application({.name = "Sandbox"})
    {
        Runic::Log::SetLevel(spdlog::level::info);

        auto window  = getWindow();
        auto context = getGraphicsContext();
        auto& layers = getLayerManager();

        layers.push_back(std::make_unique<MainLayer>(window, context));
    }
};

// Application -------------------------------------------------------------------------------------

std::unique_ptr<Runic::Application> Runic::CreateApplication()
{
    return std::make_unique<Sandbox>();
}
