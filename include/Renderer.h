#pragma once
#include "../include/Shader.h"
#include <unordered_map>

struct Character
{
    unsigned int textureId; // OpenGL texture handle
    int width;              // width of the character in pixels
    int height;             // height of the character in pixels
    int advance;            // horizontal advance after this character
    float u0;               // texture coordinate left
    float v0;               // texture coordinate bottom
    float u1;               // texture coordinate right
    float v1;               // texture coordinate top
};

class Renderer
{
private:
    void initOpenGL(void);
    void loadFontAtlas(const std::string &texturePath, int atlasWidth, int atlasHeight, int cellWidth, int cellHeight, int columns, int rows);
    void printCharacters();
    void calculateFontVertices(
        const std::string &text, float x, const float y,
        const float scale, std::vector<float> &vertices);

    unsigned int vao, vbo;
    unsigned int textVao, textVbo;

    float windowWidth, windowHeight, width, height;
    // Shader shader;

    Shader *shader;
    Shader *textShader;

    unsigned int fontTextureId;

    std::unordered_map<char, Character> Characters;
    void setOrthographicProjection(float left, float right, float bottom, float top, float near, float far, float *matrix);

public:
    Renderer(int width, int height);
    ~Renderer();
    void initText(void);
    void render(const float *vertices, int sizeVertices);
    // void render(const std::vector<float> &vertices, int sizeVertices);
    void debugUniforms(GLuint programId);
    void setWindowSize(int newWidth, int newHeight);
    void renderText(const std::string &text, float x, float y, float scale);
};