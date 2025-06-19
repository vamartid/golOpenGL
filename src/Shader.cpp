#include "../include/Shader.h"
// #include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

// Shader::Shader()
// {
// }
// Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
// {
//     std::string vertexCode = loadFile(vertexPath);
//     std::string fragmentCode = loadFile(fragmentPath);

//     if (vertexCode.empty() || fragmentCode.empty())
//     {
//         std::cerr << "Shader source is empty!" << std::endl;
//         return;
//     }

//     unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
//     unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

//     id = glCreateProgram();
//     if (id == 0)
//     {
//         std::cerr << "Failed to create shader program!" << std::endl;
//         return;
//     }

//     glAttachShader(id, vertex);
//     glAttachShader(id, fragment);
//     glLinkProgram(id);

//     int success;
//     glGetProgramiv(id, GL_LINK_STATUS, &success);
//     if (!success)
//     {
//         char programInfoLog[1024];
//         glGetProgramInfoLog(id, sizeof(programInfoLog), nullptr, programInfoLog);
//         std::cerr << "Shader Linking Failed:\n"
//                   << programInfoLog << std::endl;
//     }

//     glDeleteShader(vertex);
//     glDeleteShader(fragment);
// }

// Shader::~Shader()
// {
//     glDeleteProgram(id);
// }

// void Shader::use() const
// {
//     glUseProgram(id);
// }

// unsigned int Shader::getID() const
// {
//     return id;
// }

// unsigned int Shader::compileShader(unsigned int type, const std::string &source)
// {
//     unsigned int shader = glCreateShader(type);
//     const char *src = source.c_str();
//     glShaderSource(shader, 1, &src, nullptr);
//     glCompileShader(shader);

//     int success;
//     glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//     if (!success)
//     {
//         char infoLog[1024];
//         glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
//         std::cerr << "Shader Compilation Failed (" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "):\n"
//                   << infoLog << std::endl;
//     }

//     return shader;
// }

// std::string Shader::loadFile(const std::string &path)
// {
//     std::ifstream file(path);
//     if (!file.is_open())
//     {
//         std::cerr << "Failed to open shader file: " << path << std::endl;
//         return "";
//     }

//     std::stringstream buffer;
//     buffer << file.rdbuf();
//     return buffer.str();
// }

// void Shader::setVec2(const std::string &name, float x, float y) const
// {
//     glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
// }

// void Shader::printUniformLocations() const
// {
//     GLint resLoc = glGetUniformLocation(id, "uResolution");
//     if (resLoc == -1)
//         std::cerr << "Warning: uniform 'uResolution' not found!" << std::endl;
//     else
//         std::cout << "uResolution location: " << resLoc << std::endl;

//     GLint gridLoc = glGetUniformLocation(id, "uGridSize");
//     if (gridLoc == -1)
//         std::cerr << "Warning: uniform 'uGridSize' not found!" << std::endl;
//     else
//         std::cout << "uGridSize location: " << gridLoc << std::endl;
// }

#include <iostream>

Shader::Shader(const char *vertexSourceOrPath, const char *fragmentSourceOrPath)
{
    std::string vertexSource = loadSourceIfFile(vertexSourceOrPath);
    std::string fragmentSource = loadSourceIfFile(fragmentSourceOrPath);

    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    int success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(id, 1024, NULL, infoLog);
        std::cerr << "Error linking shader program:\n"
                  << infoLog << std::endl;
    }

    glDetachShader(id, vertex);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::use()
{
    glUseProgram(id);
}

void Shader::setUniform1f(const std::string &name, float value)
{
    int location = glGetUniformLocation(id, name.c_str());
    glUniform1f(location, value);
}

void Shader::setUniformMat4(const std::string &name, const float *matrix)
{
    int location = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix);
}

unsigned int Shader::compileShader(GLenum type, const std::string &source)
{
    unsigned int shader = glCreateShader(type);
    const char *src = source.c_str();

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "Error compiling "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader:\n"
                  << infoLog << std::endl;
    }

    return shader;
}

void Shader::setInt(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
void Shader::setVec2(const std::string &name, float x, float y)
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

bool Shader::isFilePath(const std::string &path)
{
    std::ifstream f(path);
    return f.good();
}

std::string Shader::loadSourceIfFile(const char *pathOrSource)
{
    if (isFilePath(pathOrSource))
    {
        // File exists — read its content
        std::ifstream file(pathOrSource);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    // Otherwise — it's already a literal source code
    return pathOrSource;
}