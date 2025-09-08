#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

namespace Runic
{

enum class CameraMove : uint8_t
{
    Up,
    Left,
    Down,
    Right,
};

enum class CameraZoom : uint8_t
{
    In,
    Out,
};

/** A free-fly WASD camera with +/- 89 degrees pitch clamp and 1 - 45 degrees fov clamp. */
class Camera
{
public:
    explicit Camera(float moveSpeed = 2.5F, float rotateSpeed = 0.5F, float zoomSpeed = 1.0F);

    bool invert = true;

    void setAspect(float aspect) { _aspect = aspect; }

    void moveForward(float amount);
    void moveBackward(float amount);
    void moveLeft(float amount);
    void moveRight(float amount);
    void moveUp(float amount);
    void moveDown(float amount);

    void rotateHorizontal(float amount);
    void rotateVertical(float amount);

    void zoom(float amount);

    glm::mat4 viewMatrix() const;
    glm::mat4 projectionMatrix() const;

private:
    float _moveSpeed;
    float _rotateSpeed;
    float _zoomSpeed;

    // camera body orientation
    glm::vec3 _position = {0.0F, 0.0F, 3.0F};
    glm::vec3 _front    = {0.0F, 0.0F, -1.0F};
    glm::vec3 _up       = {0.0F, 1.0F, 0.0F};

    // rotation
    float _yaw   = -90.0F;
    float _pitch = 0.0F;

    // perspective
    float _fov    = 45.0F;
    float _aspect = 1.0F;

    void updateDirection();
};

} // namespace Runic
