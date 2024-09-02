/**
 * @file DebugText.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "DebugText.hpp"
#include "DebugScreen.hpp"
#include "GRY_Game.hpp"

DebugText::DebugText(DebugScreen* debugScreen, SDL_Color color) :
    font(debugScreen->font), game(debugScreen->game), color(color) {
}

DebugText::~DebugText() { SDL_DestroyTexture(texture); texture = nullptr; }

/**
 * @details
 * This function is slow, due to the need to destroy and load the texture.
 * It should only be used in `process`, and only when the data needs to be updated. 
 */
void DebugText::updateText(const char *text) {
    /* Update the texture */
    if (texture) { SDL_DestroyTexture(texture);}
    texture = game->getSDL().loadTextTexture(text, font, color);

    /* Update dimensions */
    int textureWidth; int textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
    width = (float)textureWidth;
    height = (float)textureHeight;
}

void DebugText::render(float x, float y) const {
    SDL_FRect dstRect{ x, y, width, height };
    SDL_RenderTexture(game->getSDL().getRenderer(), texture, NULL, &dstRect);
}

void DebugText::process() {
    if (texture) { return; }

    updateText("No debug info");
}
