#include "entity.hpp"

Entity::Entity(const Model& model) : _model(model)
{
    transformation.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

void Entity::draw(Shader& shader)
{
    shader.useProgram();
    shader.setMat4F("u_world", glm::value_ptr(transformation.getWorldMatrix()));
    _model.draw(shader);
}