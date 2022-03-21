#include "scene.hpp"

Scene::Scene(Shader& shader, Shader& wireframeShader) : _shader(shader), _wireframeShader(wireframeShader), camera(glm::vec3(0.0f, 0.0f, 0.0f), 0.02f)
{
    float fov = 45.0f;
    _projectionMatrix = glm::perspective(glm::radians(fov), (float)1600/(float)900, 0.1f, 50.0f);

    // entities init
    for(unsigned int i = 0; i < 5; ++i)
    {
        for(unsigned int j = 0; j < 5; ++j)
        {
            _entities.emplace_back(Entity(Model("../ressource/model/wall/wall.obj")));
            _entities[_entities.size()-1].transformation.setPosition(glm::vec3(i, 0.0f, j));
            _entities[_entities.size()-1].transformation.setScale(glm::vec3(1.0f, 0.3f, 1.0f));
        }
    }
    for(unsigned int i = 0; i < 5; ++i)
    {
        _entities.emplace_back(Entity(Model("../ressource/model/wall/wall.obj")));
        _entities[_entities.size()-1].transformation.setPosition(glm::vec3(i, 0.5f, 5.0f));
    }
    _entities.emplace_back(Entity(Model("../ressource/model/lamp/lamp.obj")));
    _entities[_entities.size()-1].transformation.setScale(glm::vec3(0.05f, 0.05f, 0.05f));
    _entities[_entities.size()-1].transformation.setPosition(glm::vec3(2.0f, 0.2f, 3.0f));

    _entities.emplace_back(Entity(Model("../ressource/model/pot/pot.obj")));
    _entities[_entities.size()-1].transformation.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    _entities[_entities.size()-1].transformation.setPosition(glm::vec3(2.0f, 0.3f, 4.0f));

    _entities.emplace_back(Entity(Model("../ressource/model/diamond/diamond.obj")));
    _entities[_entities.size()-1].transformation.setScale(glm::vec3(0.07f, 0.07f, 0.07f));
    _entities[_entities.size()-1].transformation.setPosition(glm::vec3(2.0f, 0.7f, 4.0f));
    
    // directional light
    _directionalLight.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
    _directionalLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    _directionalLight.diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
    _directionalLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    // point lights
    for(unsigned int i = 0; i < 4; ++i)
    {
        PointLight pointLight;
        pointLight.position = glm::vec3(5.0f, 1.0f, i*2);
        pointLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
        pointLight.diffuse = glm::vec3(0.7f, 0.5f, 0.1f);
        pointLight.specular = glm::vec3(1.0f, 0.752f, 0.278f);
        _pointLights.push_back(pointLight);
    }
    
    // spot light
    _spotLight.angle = glm::cos(0.4f);
    _spotLight.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    _spotLight.diffuse = glm::vec3(0.7f, 0.4f, 0.0f);
    _spotLight.specular = glm::vec3(1.0f, 0.6f, 0.0f);

    initShader();
}

void Scene::update()
{
    _shader.useProgram();

    _shader.setVec3F("u_spotLight.position", glm::value_ptr(glm::vec3(2.0f, 2.0f, 3.0f)));
    _shader.setVec3F("u_spotLight.direction", glm::value_ptr(glm::vec3(0.0f, -1.0f, 0.5f)));

    //_shader.setVec3F("u_spotLight.position", glm::value_ptr(camera.getPosition()));
    //_shader.setVec3F("u_spotLight.direction", glm::value_ptr(camera.getTarget()));

    _shader.setMat4F("u_view", glm::value_ptr(camera.getViewMatrix()));
    _shader.setMat4F("u_projection", glm::value_ptr(_projectionMatrix));
    _shader.setVec3F("u_cameraPosition", glm::value_ptr(camera.getPosition()));

    _wireframeShader.useProgram();
    _wireframeShader.setMat4F("u_view", glm::value_ptr(camera.getViewMatrix()));
    _wireframeShader.setMat4F("u_projection", glm::value_ptr(_projectionMatrix));

    _entities[_entities.size()-1].transformation.rotate(0.0f, 0.01f, 0.0f);
    for(unsigned int i = 0; i < _entities.size(); ++i)
    {
        _entities[i].draw(_shader, _wireframeShader, true);
    }
}

void Scene::initShader()
{
    _shader.useProgram();
    _shader.set1F("u_material.shininess", 64.0f);

    // directional light
    _shader.setVec3F("u_directionalLight.direction", glm::value_ptr(_directionalLight.direction));
    _shader.setVec3F("u_directionalLight.ambiant", glm::value_ptr(_directionalLight.ambient));
    _shader.setVec3F("u_directionalLight.diffuse", glm::value_ptr(_directionalLight.diffuse));
    _shader.setVec3F("u_directionalLight.specular", glm::value_ptr(_directionalLight.specular));

    // point lights
    for(unsigned int i = 0; i < _pointLights.size(); ++i)
    {
        std::string name = "u_pointLights[" + std::to_string(i) + "]";
        _shader.setVec3F(name + ".position", glm::value_ptr(_pointLights[i].position));
        _shader.setVec3F(name + ".ambiant", glm::value_ptr(_pointLights[i].ambient));
        _shader.setVec3F(name + ".diffuse", glm::value_ptr(_pointLights[i].diffuse));
        _shader.setVec3F(name + ".specular", glm::value_ptr(_pointLights[i].specular));
        _shader.set1F(name + ".constant", 1.0f);
        _shader.set1F(name + ".linear", 0.09f);
        _shader.set1F(name + ".quadratic", 0.032f);
    }

    // spot light
    _shader.set1F("u_spotLight.angle", _spotLight.angle);
    _shader.setVec3F("u_spotLight.ambiant", glm::value_ptr(_spotLight.ambient));
    _shader.setVec3F("u_spotLight.diffuse", glm::value_ptr(_spotLight.diffuse));
    _shader.setVec3F("u_spotLight.specular", glm::value_ptr(_spotLight.specular));
    _shader.set1F("u_spotLight.constant", 1.0f);
    _shader.set1F("u_spotLight.linear", 0.09f);
    _shader.set1F("u_spotLight.quadratic", 0.032f);

}