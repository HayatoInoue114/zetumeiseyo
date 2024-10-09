#include "FPSCounter.h"

void FPSCounter::Initialize()
{
    frameCount_ = 0;
    lastTime_ = std::chrono::steady_clock::now();
    fps_ = 0.0f;
}

void FPSCounter::Update()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime_).count();
    lastTime_ = currentTime;

    frameCount_++;
    if (elapsedTime >= 1000) {
        fps_ = static_cast<double>(frameCount_) / (elapsedTime / 1000.0);
        frameCount_ = 0;
    }
}
