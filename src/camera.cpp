#include "camera.hpp"

Camera::Camera(const glm::vec3& position, float speed) : _position(position), _speed(speed)
{
    _target = glm::vec3(0.0f, 0.0f, -1.0f);
    _up = glm::vec3(0.0f, 1.0f, 0.0f);
    _yawAngle = PI/2.0f;
    _pitchAngle = 0.0f;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(_position, _position + _target, _up);
}

void Camera::processKeyboard(Direction direction)
{
    switch(direction)
    {
        case(FORWARD) : _position += _speed * _target; break;
        case(BACKWARD) : _position -= _speed * _target; break;
        case(LEFT) : _position -= _speed * glm::normalize(glm::cross(_target, _up)); break;
        case(RIGHT) : _position += _speed * glm::normalize(glm::cross(_target, _up)); break;
        case(UP) : _position += _speed * _up; break;
    }
}

void Camera::processMouseMouvements(float xOffset, float yOffset)
{
    _yawAngle += xOffset;
    _pitchAngle += yOffset;
    if(_pitchAngle < -2.5f/2.0f)
    {
        _pitchAngle = -2.5f/2.0f;
    }
    else if(_pitchAngle > 2.5f/2.0f)
    {
        _pitchAngle = 2.5f/2.0f;
    }
    _target.x = cos(_yawAngle) * cos(_pitchAngle);
    _target.y = sin(_pitchAngle);
    _target.z = -sin(_yawAngle);
}

const glm::vec3& Camera::getPosition() const
{
    return _position;
}

const glm::vec3& Camera::getTarget() const
{
    return _target;
}