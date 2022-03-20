#include "texture.hpp"

Texture::Texture(const std::string& texturePath)
{
    unsigned char* data = load(texturePath);
    if(data)
    {
        std::cout << "# texture loaded :\n"
        << " - path : \""<< texturePath << "\"\n"
        << " - width : " << _width << "\n"
        << " - height : " << _height << "\n"
        << " - channels : " << _channels
        << std::endl;

        GLenum format;
        if (_channels == 1)
            format = GL_RED;
        else if (_channels == 3)
            format = GL_RGB;
        else if (_channels == 4)
            format = GL_RGBA;

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "# could not load texture : \"" << texturePath << "\"" << std::endl;
    }
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

unsigned char* Texture::load(const std::string& texturePath)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
    if(data)
    {
        _width = width;
        _height = height;
        _channels = channels;
    }
    return data;
}