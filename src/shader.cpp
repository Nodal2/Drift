#include "shader.hpp"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vertexShaderSource, fragmentShaderSource;
    unsigned int vertexReadSuccess = readFile(vertexShaderPath, vertexShaderSource);
    unsigned int fragmentReadSuccess = readFile(fragmentShaderPath, fragmentShaderSource);
    if(vertexReadSuccess && fragmentReadSuccess)
    {
        unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
        unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

        if(!vertexShader || !fragmentShader)
        {
            std::cout << "# shader program linking stopped, shader compilation failed" << std::endl;
        }
        else
        {
            // shader program creation and linking
            id = createProgram(vertexShader, fragmentShader);
        }
    }
    else
    {
        if(!vertexReadSuccess)
        {
            std::cerr << "# vertex shader file reading failed, path provided : " << vertexShaderPath << std::endl;
        }
        if(!fragmentReadSuccess)
        {
            std::cerr << "# fragment shader file reading failed, path provided : " << fragmentShaderPath << std::endl;
        }
    }
}

void Shader::useProgram()
{
    glUseProgram(id);
}

void Shader::deleteProgram()
{
    glDeleteProgram(id);
}

void Shader::set1I(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set1F(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3F(const std::string& name, const float* vector)
{
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, vector);
}

void Shader::setMat4F(const std::string& name, const float* matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, matrix);
}

unsigned int Shader::readFile(const std::string& shaderPath, std::string& shaderSource) const
{
    std::ifstream file;
    file.open(shaderPath);
    
    if( file.is_open() )
    {
        std::string line;
        while(std::getline(file, line))
        {
            shaderSource.append(line);
            shaderSource.append("\n");
        }
        file.close();
        return 1;
    }
    return 0;
}

unsigned int Shader::compileShader(const std::string& shaderSource, unsigned int type) const
{
    unsigned int shader;
    const char* source_c = shaderSource.c_str();
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source_c, NULL);
    glCompileShader(shader);

    int success;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cerr << "# shader compilation failed";
        if(type == GL_VERTEX_SHADER)
        {
            std::cerr << " [vertex shader] : " << log;
        }
        else if(type == GL_FRAGMENT_SHADER)
        {
            std::cerr << " [fragment shader] : " << log;
        }
        return 0;
    }
    return shader;
}

unsigned int Shader::createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int program;
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cerr << "# shader program linking failed : " << log << std::endl;
        return 0;
    }
    glValidateProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}