#ifndef DRIFT_SHADER
#define DRIFT_SHADER

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>

class Shader
{
public :
    Shader(const std::string&, const std::string&);
    void useProgram();
    void deleteProgram();
    void set1I(const std::string&, int);
    void set1F(const std::string&, float);
    void setVec3F(const std::string&, const float*);
    void setMat4F(const std::string&, const float*);
    unsigned int id;

private :
    unsigned int readFile(const std::string&, std::string&) const;
    unsigned int compileShader(const std::string&, unsigned int) const;
    unsigned int createProgram(unsigned int, unsigned int);
};

#endif