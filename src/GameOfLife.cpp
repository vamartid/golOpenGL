#include "../include/GameOfLife.h"

#include <random>
#include <thread>
#include <algorithm>
#include <iostream>

template class GameOfLife<160, 90>; // This instantiates this combination

template <int WIDTH, int HEIGHT>
GameOfLife<WIDTH, HEIGHT>::GameOfLife(int threadCount)
    : threads(threadCount),
      barrier(threadCount + 1), // +1 for main thread
      running(false),
      stepRequested(false)
{
    initialize();
}

template <int WIDTH, int HEIGHT>
GameOfLife<WIDTH, HEIGHT>::~GameOfLife()
{
    stop();
}

template <int WIDTH, int HEIGHT>
void GameOfLife<WIDTH, HEIGHT>::initialize()
{
    // std::uniform_int_distribution<int> dist(0, 1);
    std::mt19937 rng(std::random_device{}());
    std::bernoulli_distribution dist(0.5);
    for (int y = 1; y <= height; ++y)
        for (int x = 1; x <= width; ++x)
            (*cells)[y * paddedWidth + x] = dist(rng); // Correct interpretation
}

template <int WIDTH, int HEIGHT>
void GameOfLife<WIDTH, HEIGHT>::start()
{
    running = true;
    stepRequested = false;
    workers.reserve(threads);
    for (int t = 0; t < threads; ++t)
    {
        workers.emplace_back(&GameOfLife<WIDTH, HEIGHT>::workerThread, this, t);
    }
}

template <int WIDTH, int HEIGHT>
void GameOfLife<WIDTH, HEIGHT>::stop()
{
    if (!running)
        return;

    running = false;
    stepRequested = false;
    barrier.stop(); // <--- unblock all threads
    // Wake up threads from both barriers
    for (int i = 0; i < 2; ++i)
        barrier.arrive_and_wait();

    for (auto &t : workers)
    {
        if (t.joinable())
            t.join();
    }

    workers.clear();
}

template <int WIDTH, int HEIGHT>
void GameOfLife<WIDTH, HEIGHT>::step()
{
    stepRequested = true;
    barrier.arrive_and_wait(); // Wake workers
    barrier.arrive_and_wait(); // Wait for completion
    std::swap(cells, nextGen);
    stepRequested = false;
}

template <int WIDTH, int HEIGHT>
void GameOfLife<WIDTH, HEIGHT>::workerThread(int t)
{
    const int chunkSize = (height + threads - 1) / threads; // ceil division
    const int yStart = 1 + t * chunkSize;
    const int yEnd = std::min(height, yStart + chunkSize);

    auto index = [&](int x, int y)
    {
        return y * paddedWidth + x;
    };

    while (running && !barrier.isStopping())
    {
        barrier.arrive_and_wait(); // Wait for step start

        if (!running)
            break;

        if (stepRequested)
        {
            for (int y = yStart; y <= yEnd; ++y)
            {
                for (int x = 1; x <= width; ++x)
                {
                    int alive = 0;
                    for (int dy = -1; dy <= 1; ++dy)
                    {
                        for (int dx = -1; dx <= 1; ++dx)
                        {
                            if (dx != 0 || dy != 0)
                                alive += (*cells)[index(x + dx, y + dy)];
                        }
                    }

                    bool current = (*cells)[index(x, y)];
                    (*nextGen)[index(x, y)] = (current && (alive == 2 || alive == 3)) || (!current && alive == 3);
                }
            }
        }

        barrier.arrive_and_wait(); // End of step
    }
}

template <int WIDTH, int HEIGHT>
void GameOfLife<WIDTH, HEIGHT>::calculateVertices()
{
    // vertices.clear();
    int index = 0;
    for (int y = 1; y <= height; ++y)
    {
        for (int x = 1; x <= width; ++x)
        {
            if ((*cells)[y * paddedWidth + x])
            {
                float fx = 2.0f * (x - 1) / width - 1.0f;
                float fy = 2.0f * (y - 1) / height - 1.0f;
                vertices[++index] = fx;
                vertices[++index] = fy;
            }
        }
    }
    sizeVertices = index;
}
