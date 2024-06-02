/**
 * @file DebugTextFPS.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "DebugTextFPS.hpp"
#include "GRY_Game.hpp"

void DebugTextFPS::process() {
    double fps = game->getFPS();

    /* Change texture only if fps has changed */
    if (lastFps != fps) {
        /* Format fps to C string */
        char output[16];
        snprintf(output, 16, "FPS: %.1f", fps);

        /* Update texture */
        updateText(output);

        lastFps = fps;
    }
}
