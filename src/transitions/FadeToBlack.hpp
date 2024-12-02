/**
 * @file FadeToBlack.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief FadeToBlack
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Transition.hpp"

/**
 * @brief Fade transition that goes in and out to black.
 * 
 * @details
 * The initial phase is a fade-in effect, and the release
 * phase is a fade-out effect.
 * 
 * The transition targets the entire renderer.
 */
class FadeToBlack : public Transition {
private:

    /**
     * @brief Maximum value of RGBA levels in a color.
     * 
     */
    float MAX_RGBA_VALUE = 255;

    /**
     * @brief Time it will take for the initial phase.
     * 
     * @sa Transition
     */
    double initTime;

    /**
     * @brief Time it will take for the release phase.
     * 
     * @sa Transition
     */
    double releaseTime;

    /**
     * @brief Opacity of the fade effect.
     * 
     */
    float alphaLevel = 0.0f;
public:
    /**
     * @brief Constructor.
     * 
     * @param game @copybrief Transition::game
     * @param initTime Length of the fade-in effect in seconds.
     * @param releaseTime Length of the fade-out effect in seconds.
     */
    FadeToBlack(GRY_Game* game, double initTime, double releaseTime) : 
    Transition(game),
    initTime(initTime), 
    releaseTime(releaseTime) {}

    /**
     * @brief Default constructor with fade-in and fade-out taking 0.5 seconds each.
     * 
     * @param game @copybrief Transition::game
     */
    FadeToBlack(GRY_Game* game) : FadeToBlack(game, 0.5, 0.5) {}

    /**
     * @copydoc Transition::process
     */
    bool process() final override;

    /**
     * @copydoc Transition::isReadyToRelease
     */
    bool isReadyToRelease() final override { return alphaLevel >= MAX_RGBA_VALUE; }
};