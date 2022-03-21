#include "model.hpp"

Model::Model(const std::string& path)
{
    load(path);
}

void Model::draw(Shader& shader)
{
    for(unsigned int i = 0; i < _meshes.size(); ++i)
    {
        _meshes[i].draw(shader);
    }
}

void Model::drawWireframe(Shader& shader)
{
    for(unsigned int i = 0; i < _meshes.size(); ++i)
    {
        _meshes[i].drawWireframe(shader);
    }
}

void Model::load(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "# could not load 3D model : " << importer.GetErrorString() << std::endl;
        return;
    }
    _directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture2> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        vertex.position = position;

        if(mesh->HasNormals())
        {
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            vertex.normal = normal;
        }
        if(mesh->mTextureCoords[0])
        {
            glm::vec2 texture; // check if there is texture coords
            texture.x = mesh->mTextureCoords[0][i].x;
            texture.y = mesh->mTextureCoords[0][i].y;
            vertex.texture = texture;
        }
        else
        {
            vertex.texture = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i]; // pointeur ?
        for(unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture2> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    std::vector<Texture2> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Texture2> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& name)
{
    std::vector<Texture2> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < _loadedTextures.size(); ++j)
        {
            if(std::strcmp(_loadedTextures[j].path.data(),
            str.C_Str()) == 0)
            {
                textures.push_back(_loadedTextures[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            Texture2 texture2;
            std::string filename = std::string(str.C_Str());
            filename = _directory + '/' + filename;
            Texture texture = Texture(filename);
            texture2.id = texture.id;
            texture2.type = name;
            texture2.path = std::string(str.C_Str());
            textures.push_back(texture2);
            _loadedTextures.push_back(texture2);
        }
        
    }

    return textures;
}