#ifndef DRIFT_CAMERA
#define DRIFT_CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define PI 3.14159

class Camera
{
public :
    enum Direction
    {
        FORWARD, BACKWARD, LEFT, RIGHT, UP
    };
    Camera(const glm::vec3&, float);
    glm::mat4 getViewMatrix() const;
    void processKeyboard(Direction);
    void processMouseMouvements(float, float);
    const glm::vec3& getPosition() const;
    const glm::vec3& getTarget() const;

private :
    float _speed;
    glm::vec3 _position;
    glm::vec3 _target;
    glm::vec3 _up;
    float _yawAngle;
    float _pitchAngle;
};

#endif