#ifndef DRIFT_TRANSFORMATION
#define DRIFT_TRANSFORMATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformation
{
public :
    Transformation();
    void setPosition(const glm::vec3&);
    void setRotation(const glm::vec3&);
    void setScale(const glm::vec3&);
    const glm::vec3& getPosition() const;
    void translate(float, float, float);
    void rotate(float, float, float);
    void resize(float, float, float);
    glm::mat4 getWorldMatrix() const;

private :
    glm::vec3 _position;
    glm::vec3 _rotation;
    glm::vec3 _scale;
};

#endif