/**
 * @file DebugTextFPS.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief DebugTextFPS
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "DebugText.hpp"

/**
 * @brief Debug text that displays the FPS (frames per second).
 * 
 * @details
 * The text texture is updated every frame that the FPS changes,
 * which may cause slowdowns if the FPS is not stable.
 */
struct DebugTextFPS : public DebugText {
private:
    /**
     * @brief Fps of the last frame.
     * 
     */
    double lastFps = 0.0;
public:
    /**
     * @copydoc DebugText
     */
    DebugTextFPS(DebugScreen* debugScreen, SDL_Color color) :
        DebugText(debugScreen, color) {
    }

    /**
     * @brief Checks the FPS and updates the text if it has changed.
     * 
     */
    void process() final override;
};