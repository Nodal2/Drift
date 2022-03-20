#include "transformation.hpp"

Transformation::Transformation() : _position(glm::vec3(0.0f)), _rotation(glm::vec3(0.0f)), _scale(glm::vec3(1.0f)) {}

void Transformation::setPosition(const glm::vec3& position)
{
    _position = position;
}

void Transformation::setRotation(const glm::vec3& rotation)
{
    _rotation = rotation;
}

void Transformation::setScale(const glm::vec3& scale)
{
    _scale = scale;
}

const glm::vec3& Transformation::getPosition() const
{
    return _position;
}

void Transformation::translate(float x, float y, float z)
{
    _position.x += x;
    _position.y += y;
    _position.z += z;
}

void Transformation::rotate(float x, float y, float z)
{
    _rotation.x += x;
    _rotation.y += y;
    _rotation.z += z;
}

void Transformation::resize(float x, float y, float z)
{
    _scale.x += x;
    _scale.y += y;
    _scale.z += z;
}

glm::mat4 Transformation::getWorldMatrix() const
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, _position);
    modelMatrix = glm::rotate(modelMatrix, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, _scale);
    return modelMatrix;
}