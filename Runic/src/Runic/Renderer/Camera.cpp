#include "Runic/Renderer/Camera.hpp"

#include "glm/common.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

namespace Runic
{

void Camera::moveForward(float amount)
{
    _position += _front * amount * moveSpeed;
}

void Camera::moveBackward(float amount)
{
    _position -= _front * amount * moveSpeed;
}

void Camera::moveLeft(float amount)
{
    _position -= glm::normalize(glm::cross(_front, _up)) * amount * moveSpeed;
}

void Camera::moveRight(float amount)
{
    _position += glm::normalize(glm::cross(_front, _up)) * amount * moveSpeed;
}

void Camera::rotateHorizontal(float amount)
{
    _yaw += amount * rotateSpeed;
    updateDirection();
}

void Camera::rotateVertical(float amount)
{
    _pitch = glm::clamp(_pitch - (invert ? -1.0F : 1.0F) * amount * rotateSpeed, -89.0F, 89.0F);
    updateDirection();
}

void Camera::moveUp(float amount)
{
    glm::vec3 right = glm::normalize(glm::cross(_front, _up));
    _position -= glm::normalize(glm::cross(_front, right)) * amount * moveSpeed;
}

void Camera::moveDown(float amount)
{
    glm::vec3 right = glm::normalize(glm::cross(_front, _up));
    _position += glm::normalize(glm::cross(_front, right)) * amount * moveSpeed;
}

void Camera::updateDirection()
{
    glm::vec3 direction;
    direction.x = static_cast<float>(cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)));
    direction.y = static_cast<float>(sin(glm::radians(_pitch)));
    direction.z = static_cast<float>(sin(glm::radians(_yaw)) * cos(glm::radians(_pitch)));
    _front      = glm::normalize(direction);
}

void Camera::zoom(float amount)
{
    _fov = glm::clamp(_fov - amount * zoomSpeed, 1.0F, 45.0F);
}

glm::mat4 Camera::viewMatrix() const
{
    return glm::lookAt(_position, _position + _front, _up);
}

glm::mat4 Camera::projectionMatrix() const
{
    return glm::perspective(glm::radians(_fov), _aspect, 0.1F, 100.0F);
}

} // namespace Runic
