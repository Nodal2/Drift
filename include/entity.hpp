#ifndef DRIFT_ENTITY
#define DRIFT_ENTITY

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "transformation.hpp"
#include "model.hpp"

class Entity
{
public:
    Entity(const Model&);
    void draw(Shader&);
    Transformation transformation;
private:
    Model _model;
};

#endif