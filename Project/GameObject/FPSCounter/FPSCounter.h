#pragma once
#include <iostream>
#include <chrono>
#include <vector>

class FPSCounter {
public:
    void Initialize();

    void Update();

    double GetFPS() const {
        return fps_;
    }

private:
    int frameCount_;
    double fps_;
    std::chrono::steady_clock::time_point lastTime_;
};

