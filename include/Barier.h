#pragma once
#include <mutex>
#include <condition_variable>
class Barrier
{
    std::mutex mtx;
    std::condition_variable cv;
    int count;
    int threshold;
    int generation = 0;
    bool stopping = false;

public:
    Barrier(int n) : count(n), threshold(n) {}

    void arrive_and_wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (stopping)
            return;

        auto gen = generation;
        if (--count == 0)
        {
            generation++;
            count = threshold;
            cv.notify_all();
        }
        else
        {
            cv.wait(lock, [this, gen]
                    { return generation != gen || stopping; });
        }
    }

    void stop()
    {
        std::unique_lock<std::mutex> lock(mtx);
        stopping = true;
        cv.notify_all();
    }

    bool isStopping() const
    {
        return stopping;
    }
};
