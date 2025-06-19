#include "../include/Window.h"

#include <stdexcept>
#include <iostream>

Window::Window(int width, int height, const std::string &title)
{

    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1); // Enables vsync
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Store 'this' in the window’s user pointer
    glfwSetWindowUserPointer(window, this);

    // Set up framebuffer size callback (static)
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackGLFW);

    // Initialize OpenGL settings
    glViewport(0, 0, width, height);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Window::beginFrame()
{
    glClearColor(1.f, 1.f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::endFrame()
{
    glfwSwapBuffers(window);
}

void Window::terminate()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::setRenderer(Renderer *renderer)
{
    this->renderer = renderer;
    // renderer->setWindowSize(width, height);
}

void Window::framebufferSizeCallback(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    if (renderer)
    {
        renderer->setWindowSize(width, height);
    }
}

void Window::framebufferSizeCallbackGLFW(GLFWwindow *window, int width, int height)
{
    // GLFWwindow* is a GLFW object, not our custom Window class.
    // In constructor we saved our instance with glfwSetWindowUserPointer
    // Retrieve the ptr with glfwGetWindowUserPointer and store it to the ptr variable window
    // This way we connect the static callback back to our instance.
    Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (win)
    {
        win->framebufferSizeCallback(width, height);
    }
}