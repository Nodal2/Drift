#ifndef DRIFT_SCENE
#define DRIFT_SCENE

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "entity.hpp"
#include "camera.hpp"

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;
    float angle;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class Scene
{
public:
    Scene(Shader&);
    void update();
    Camera camera;
private:
    void initShader();
    Shader _shader;
    std::vector<Entity> _entities;
    DirectionalLight _directionalLight;
    std::vector<PointLight> _pointLights;
    SpotLight _spotLight;
    glm::mat4 _projectionMatrix;
};

#endif