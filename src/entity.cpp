#include "entity.hpp"

Entity::Entity(const Model& model) : _model(model)
{
    transformation.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

void Entity::draw(Shader& shader, Shader& wireframeShader, bool wireframeMode)
{
    shader.useProgram();
    shader.setMat4F("u_world", glm::value_ptr(transformation.getWorldMatrix()));
    if(wireframeMode)
    {
        wireframeShader.useProgram();
        transformation.resize(0.00001f, 0.00001f, 0.00001f); // resize to avoid z-fighting
        wireframeShader.setMat4F("u_world", glm::value_ptr(transformation.getWorldMatrix()));
        _model.drawWireframe(wireframeShader);
        transformation.resize(-0.00001f, -0.00001f, -0.00001f);
    }
    _model.draw(shader);
}