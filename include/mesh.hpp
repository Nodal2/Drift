#ifndef DRIFT_MESH
#define DRIFT_MESH

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
};

struct Texture2
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    Mesh(const std::vector<Vertex>&, const std::vector<unsigned int>&, const std::vector<Texture2>&);
    void draw(Shader&);
    void drawWireframe(Shader&);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2> textures;
private:
    void init();
    unsigned int _vao, _vbo, _ebo;
};

#endif