#pragma once

#include "../include/Barier.h"

#include <array>
#include <atomic>

template <int WIDTH, int HEIGHT>
class GameOfLife
{
public:
    static constexpr int width = WIDTH;
    static constexpr int height = HEIGHT;

    static constexpr int paddedWidth = width + 2;
    static constexpr int paddedHeight = height + 2;

    static constexpr int maxVertices = width * height * 2;

    GameOfLife(int threadCount = 0);
    ~GameOfLife();

    void initialize();
    void start();
    void stop();
    void step(); // trigger next generation (main thread only)
    void calculateVertices();
    std::array<float, maxVertices> vertices;
    int sizeVertices;

private:
    int threads;

    void workerThread(int threadId);

    // cells values either current or future generation
    std::array<bool, paddedWidth * paddedHeight> buffer1;
    std::array<bool, paddedWidth * paddedHeight> buffer2;

    // Pointers to current and future buffers
    std::array<bool, paddedWidth * paddedHeight> *cells = &buffer1;
    std::array<bool, paddedWidth * paddedHeight> *nextGen = &buffer2;

    std::vector<std::thread> workers;
    Barrier barrier;

    std::atomic<bool> running;
    std::atomic<bool> stepRequested;
};