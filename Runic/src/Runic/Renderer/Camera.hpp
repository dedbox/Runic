#pragma once

#include "glm/glm.hpp"

namespace Runic
{
class OrthographicCamera
{
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    const glm::vec3& getPosition() const { return _position; }

    void setPosition(const glm::vec3& position)
    {
        _position = position;
        recalculateViewMatrix();
    }

    float getRotation() const { return _rotation; }

    void setRotation(const float rotation)
    {
        _rotation = rotation;
        recalculateViewMatrix();
    }

    const glm::mat4& getProjectionMatrox() const { return _projectionMatrix; }
    const glm::mat4& getViewMatrox() const { return _viewMatrix; }
    const glm::mat4& getViewProjectionMatrix() const { return _viewProjectionMatrix; }

private:
    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _viewProjectionMatrix;

    glm::vec3 _position;
    float _rotation{0.0f};

    void recalculateViewMatrix();
};
} // Runic
