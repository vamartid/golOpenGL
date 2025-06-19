#include "../include/Shader.h"

#include <fstream>
#include <sstream>
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