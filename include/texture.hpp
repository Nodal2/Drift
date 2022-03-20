#ifndef DRIFT_TEXTURE
#define DRIFT_TEXTURE

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include <string>

class Texture
{
public :
    Texture(const std::string&);
    void bind();
    unsigned int id;

private :
    unsigned char* load(const std::string&);
    unsigned int _width, _height, _channels;
};

#endif