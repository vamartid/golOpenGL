#pragma once

#include <glad/glad.h>
#include <string>

class Shader
{
private:
    unsigned int id;

    unsigned int compileShader(GLenum type, const std::string &source);
    void linkProgram(unsigned int vertex, unsigned int fragment);

    static bool isFilePath(const std::string &path);
    static std::string loadSourceIfFile(const char *pathOrSource);

public:
    Shader(const char *vertexSourceOrPath, const char *fragmentSourceOrPath);
    ~Shader();

    void use();

    unsigned int getId() const { return id; }
    void setUniformMat4(const std::string &name, const float *matrix);
    void setUniform1f(const std::string &name, float value);
    void setInt(const std::string &name, int value);
    void setVec3(const std::string &name, float x, float y, float z);
    void setVec2(const std::string &name, float x, float y);
    // … additional methods for other uniform types
};

// class Shader
// {
// public:
//     Shader();
//     Shader(const std::string &vertexPath, const std::string &fragmentPath);
//     ~Shader();

//     void use() const;
//     unsigned int getID() const;
//     void setVec2(const std::string &name, float x, float y) const;
//     void Shader::printUniformLocations() const;

// private:
//     unsigned int id;
//     std::string loadFile(const std::string &path);
//     unsigned int compileShader(unsigned int type, const std::string &source);
// };
