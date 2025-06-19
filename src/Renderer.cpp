#include "../include/Renderer.h"
#include "../include/Paths.h"

#include <glad/glad.h> // <- for glTexParameteri, GL_WRAP_S, etc.
#include <iostream>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Renderer::Renderer(int width, int height) : width(float(width)), height(float(height))
{
    initOpenGL(); // init OpenGL for point rendering
    initText();   // init OpenGL for text rendering and load texture atlas file
    // 3️⃣ Initialize shaders
    shader = new Shader(Paths::shaderVertPath, Paths::shaderFragPath);
    textShader = new Shader(Paths::textShaderVertPath, Paths::textShaderFragPath);
}

Renderer::~Renderer()
{
    delete shader;
    delete textShader;
}
void Renderer::initOpenGL()
{
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable depth-test if needed
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set up main renderer’s VAO and VBO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, int(width * height) * sizeof(float) * 2, NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::initText()
{
    glGenVertexArrays(1, &textVao);
    glBindVertexArray(textVao);

    glGenBuffers(1, &textVbo);
    glBindBuffer(GL_ARRAY_BUFFER, textVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4 * 540, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load font texture…
    loadFontAtlas(Paths::fontAsset, 78, 70, 6, 10, 13, 7);
}

void Renderer::render(const float *vertices, int sizeVertices)
{
    // Set clear color to black with full opacity
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Then clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVertices * sizeof(float), vertices);

    // Activate main renderer’s shader
    shader->use();
    shader->setVec2("uResolution", windowWidth, windowHeight);
    shader->setVec2("uGridSize", width, height);

    glDrawArrays(GL_POINTS, 0, sizeVertices / 2);
}

void Renderer::calculateFontVertices(
    const std::string &text, float x, const float y,
    const float scale, std::vector<float> &vertices)
{
    float startX = x;
    size_t length = text.length();
    for (size_t i = 0; i < length; i++)
    {
        char c = text[i];
        if (Characters.find(c) == Characters.end())
            continue;

        const Character &ch = Characters.at(c);

        float w = ch.width * scale;
        float h = ch.height * scale;

        float xpos = x;
        float ypos = y;

        // 6 vertices per quad (two triangles)
        float quad[6][4] = {
            {xpos, ypos + h, ch.u0, ch.v1},
            {xpos, ypos, ch.u0, ch.v0},
            {xpos + w, ypos, ch.u1, ch.v0},

            {xpos + w, ypos, ch.u1, ch.v0},
            {xpos + w, ypos + h, ch.u1, ch.v1},
            {xpos, ypos + h, ch.u0, ch.v1},
        };

        for (int v = 0; v < 6; v++)
        {
            vertices[(i * 6 + v) * 4 + 0] = quad[v][0];
            vertices[(i * 6 + v) * 4 + 1] = quad[v][1];
            vertices[(i * 6 + v) * 4 + 2] = quad[v][2];
            vertices[(i * 6 + v) * 4 + 3] = quad[v][3];
        }

        x += (ch.advance) * scale;
    }
}
void Renderer::renderText(const std::string &text, float x, float y, float scale)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glDisable(GL_PROGRAM_POINT_SIZE);
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(textVao);
    glBindBuffer(GL_ARRAY_BUFFER, textVao);
    textShader->use();
    // Set projection
    float projection[16];
    setOrthographicProjection(0.0f, (float)windowWidth, (float)windowHeight, 0.0f, -1.0f, 1.0f, projection);

    // Set text color
    textShader->setInt("textTexture", 0);
    textShader->setVec3("textColor", 1.0f, 1.0f, 0.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontTextureId);
    textShader->setUniformMat4("projection", projection);
    // Update buffer with text’s quads
    size_t length = text.size();
    std::vector<float> vertices(length * 6 * 4); // Each char = 6 vertices, each vertex = 4 floats
    calculateFontVertices(text, x, y, scale, vertices);
    // Update buffer in a single call
    glBufferSubData(GL_ARRAY_BUFFER, 0, length * 6 * 4 * sizeof(float), vertices.data());
    // Finally draw
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(length * 6));
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_PROGRAM_POINT_SIZE);
}

void Renderer::setWindowSize(int newWidth, int newHeight)
{
    this->windowWidth = static_cast<float>(newWidth);
    this->windowHeight = static_cast<float>(newHeight);
}

void Renderer::debugUniforms(GLuint programId)
{
    GLint resLoc = glGetUniformLocation(programId, "uResolution");
    if (resLoc == -1)
    {
        std::cerr << "Unable to find uResolution in the shader!" << std::endl;
    }
    GLfloat resValues[2];
    glGetUniformfv(programId, resLoc, resValues);
    std::cout << "uResolution = (" << resValues[0] << ", " << resValues[1] << ")" << std::endl;

    GLint gridLoc = glGetUniformLocation(programId, "uGridSize");
    if (gridLoc == -1)
    {
        std::cerr << "Unable to find uResolution in the shader!" << std::endl;
    }
    GLfloat gridValues[2];
    glGetUniformfv(programId, gridLoc, gridValues);
    std::cout << "uGridSize = (" << gridValues[0] << ", " << gridValues[1] << ")" << std::endl;
}

void Renderer::loadFontAtlas(const std::string &texturePath, int atlasWidth, int atlasHeight, int cellWidth, int cellHeight, int columns, int rows)
{
    const std::string CHAR_ORDER = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-=()[]{}<>/*:#%!?.,'\"@&$ ";
    int width, height, channels;

    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!data)
    {
        std::cerr << "Failed to load font texture" << std::endl;
        return;
    }

    glGenTextures(1, &fontTextureId);
    // Bind your font texture to the active texture unit
    glBindTexture(GL_TEXTURE_2D, fontTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    STBI_FREE(data);

    // Calculate texture coordinates for each character
    int numCharacters = (int)CHAR_ORDER.size();

    float epsilonU = 0.5f / (float)atlasWidth;
    float epsilonV = 0.5f / (float)atlasHeight;

    for (int i = 0; i < numCharacters; i++)
    {
        char c = CHAR_ORDER[i];
        int row = i / columns;
        int col = i % columns;

        float u0 = (float)col * (float)cellWidth / (float)atlasWidth + epsilonU;
        float v0 = (float)row * (float)cellHeight / (float)atlasHeight + epsilonV;

        float u1 = u0 + (float)cellWidth / (float)atlasWidth - 2.0f * epsilonU;
        float v1 = v0 + (float)cellHeight / (float)atlasHeight - 2.0f * epsilonV;

        Characters[c] = {
            fontTextureId,
            cellWidth,
            cellHeight,
            cellWidth, // or custom advance if you wish
            u0, v0, u1, v1};
    }
    // printCharacters();
}
void Renderer::printCharacters()
{
    std::cout << "Characters texture mapping:" << std::endl;

    for (const auto &pair : Characters)
    {
        char c = pair.first;
        const Character &ch = pair.second;

        std::cout << "'" << c << "' -> "
                  << "Width: " << ch.width << ", Height: " << ch.height << ", Advance: " << ch.advance << ", "
                  << "U0: " << ch.u0 << ", V0: " << ch.v0 << ", "
                  << "U1: " << ch.u1 << ", V1: " << ch.v1 << std::endl;
    }
}
void Renderer::setOrthographicProjection(float left, float right, float bottom, float top, float near, float far, float *matrix)
{
    // matrix is a float[16] in column-major order
    // orthographic projection matrix formula:
    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glOrtho.xml

    float rl = right - left;
    float tb = top - bottom;
    float fn = far - near;

    matrix[0] = 2.0f / rl;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = 2.0f / tb;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = -2.0f / fn;
    matrix[11] = 0.0f;

    matrix[12] = -(right + left) / rl;
    matrix[13] = -(top + bottom) / tb;
    matrix[14] = -(far + near) / fn;
    matrix[15] = 1.0f;
}
