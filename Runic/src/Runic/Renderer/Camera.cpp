#include "Runic/Renderer/Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace Runic
{
OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top)
    : _projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
    , _viewMatrix(1.0f)
    , _viewProjectionMatrix(_projectionMatrix * _viewMatrix),
      _position(0.0f, 0.0f, 0.0f) {}

void OrthographicCamera::recalculateViewMatrix()
{
    const glm::mat4 transform{
        glm::translate(glm::mat4(1.0f), _position)
        * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0, 0, 1))
    };
    _viewMatrix = glm::inverse(transform);
    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}
} // Runic
