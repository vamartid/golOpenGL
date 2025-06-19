#pragma once

#include "../include/Renderer.h"

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    int width, height;
    static Window &getInstance(int width = 800, int height = 600, const std::string &title = "Game")
    {
        static Window instance(width, height, title);
        return instance;
    }

    void pollEvents();
    bool shouldClose() const;
    void beginFrame();
    void endFrame();
    void terminate();
    void setRenderer(Renderer *renderer);

    void framebufferSizeCallback(int width, int height);

    GLFWwindow *getGLFWwindow() const { return window; }

private:
    Window(int width, int height, const std::string &title);
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    GLFWwindow *window;

    static void framebufferSizeCallbackGLFW(GLFWwindow *window, int width, int height);
    Renderer *renderer;
};
