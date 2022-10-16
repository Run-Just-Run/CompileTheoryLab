#pragma once
#include <iostream>
#include <chrono>
#include <thread>

struct Timer
{
    std::chrono::time_point<std::chrono::system_clock> start,end;
    std::chrono::duration<float> duration;

    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end -start;

        float ms = duration.count() * 1000.0f;
        std::cout<<ms<<" "<<"ms"<<std::endl;
    }
};



