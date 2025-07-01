/**
 * @file FadeToBlack.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "FadeToBlack.hpp"
#include "GRY_Game.hpp"
#include "GRY_Log.hpp"
#include "SDL3/SDL_render.h"

bool FadeToBlack::process() {
    /* Initial phase */
    if (!releasing && alphaLevel < MAX_RGBA_VALUE) {
        alphaLevel += (float)((MAX_RGBA_VALUE/initTime) * game->getDelta());
        if (alphaLevel > MAX_RGBA_VALUE) {alphaLevel = MAX_RGBA_VALUE;}
    }
    /* Release phase */
    else if (releasing && alphaLevel > 0) {
        alphaLevel -= (float)((MAX_RGBA_VALUE/releaseTime) * game->getDelta());
    }
    /* Finished */
    if (alphaLevel <= 0) {
        GRY_Log("[FadeToBlack] Transition complete.\n");
        return true;
    }

    /* Fill screen with black at the current alpha level. */
    SDL_SetRenderDrawColor(game->getVideo().getRenderer(), 0,0,0, (Uint8)alphaLevel);
    SDL_RenderFillRect(game->getVideo().getRenderer(), NULL);
    return false;
}