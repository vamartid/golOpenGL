#include "../include/Window.h"
#include "../include/GameOfLife.h"
#include "../include/Renderer.h"

#include <thread>
#include <chrono>
#include <iostream>

// static constexpr int width = 160, height = 60;

int main()
{
    int winWidth = 1920, winHeight = 1080;

    double lastTime = glfwGetTime();
    int nbFrames = 0;
    int generations = 0;
    double fps = 0;
    float x = 1920 / 2, y = 1080 / 2; // move down by 50px

    // Initialize window and OpenGL
    Window &window = Window::getInstance(winWidth, winHeight, "Multithreaded Game of Life");

    // threads
    unsigned int n = std::thread::hardware_concurrency();
    if (n == 0)
        n = 1; // fallback if detection fails

    // init game object
    GameOfLife<160, 90> game(n); // width, height, thread count

    // init renderer and asign it to window
    Renderer renderer(game.width, game.height);
    renderer.setWindowSize(winWidth, winHeight);
    window.setRenderer(&renderer);
    renderer.initText();

    game.start();
    while (!window.shouldClose())
    {
        game.step();
        window.pollEvents();

        window.beginFrame();
        game.calculateVertices();

        renderer.render(game.vertices.data(), game.sizeVertices);
        std::string textGen = "Generations: " + std::to_string(generations);
        std::string textFps = "FPS: " + std::to_string(fps);
        renderer.renderText(textGen, 40, y, 2.0f);
        renderer.renderText(textFps, 40, y + 40, 2.0f);
        window.endFrame();

        // Count and calculate FPS
        nbFrames++;
        generations++;
        double currentTime = glfwGetTime();

        if (currentTime - lastTime >= 1.0) // If 1-second passed
        {
            fps = double(nbFrames) / (currentTime - lastTime);
            nbFrames = 0;
            lastTime = currentTime;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }

    //  Clean shutdown
    game.stop();
    window.shouldClose();
    return 0;
}
