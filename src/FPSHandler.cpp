/**
 * @file FPSHandler.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "FPSHandler.hpp"
#include "SDL3/SDL.h"

FPSHandler::FPSHandler(const unsigned int TARGET_FPS) :
    TARGET_FPS(TARGET_FPS),
    frameLength(1000 / TARGET_FPS),
    deltaTime(1.0 / TARGET_FPS),
    minDelta(1.0 / TARGET_FPS) {
}

void FPSHandler::frameStart() {
    start = SDL_GetPerformanceCounter();
    msStart = SDL_GetTicks();
}

void FPSHandler::computeDelta() {
    /* Delta = current time - frame start time. Divide by performance freq. to get in seconds. */
    deltaTime = (double)(SDL_GetPerformanceCounter() - start) / SDL_GetPerformanceFrequency();

    /* Ensure delta is between maxDelta and minDelta. */
    if (deltaTime > maxDelta) { deltaTime = maxDelta; }
    else if (deltaTime < minDelta) { deltaTime = minDelta; }
}

/**
 * @details
 * Uses `SDL_Delay()` so that the total time of the current frame will be equal to
 * `frameLength`. If it is already greater than or equal to `frameLength`, does nothing.
 */
void FPSHandler::delay() {
    Uint64 timeSinceFrameStart = SDL_GetTicks() - msStart;
    if (timeSinceFrameStart < frameLength) {
        SDL_Delay(frameLength - timeSinceFrameStart);
    }
}