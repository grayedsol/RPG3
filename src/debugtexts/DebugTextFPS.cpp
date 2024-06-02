/**
 * @file DebugTextFPS.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "DebugTextFPS.hpp"
#include "GRY_Game.hpp"

void DebugTextFPS::process() {
    double newFps = game->getFPS();

    /* Change texture only if fps has changed */
    if (fps != newFps) {
        /* Format newFps to C string */
        char output[16];
        snprintf(output, 16, "FPS: %.1f", newFps);

        /* Update texture */
        updateText(output);

        fps = newFps;
    }
}
