#ifndef DRIFT_MODEL
#define DRIFT_MODEL

#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.hpp"
#include "texture.hpp"

class Model
{
public:
    Model(const std::string&);
    void draw(Shader&);

private:
    void load(const std::string&);
    void processNode(aiNode*, const aiScene*);
    Mesh processMesh(aiMesh*, const aiScene*);
    std::vector<Texture2> loadMaterialTextures(aiMaterial*, aiTextureType, const std::string&);
    std::vector<Mesh> _meshes;
    std::string _directory;
    std::vector<Texture2> _loadedTextures;
};

#endif