/**
 * @file DebugScreen.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "DebugScreen.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"
#include "SDL3_ttf/SDL_ttf.h"

DebugScreen::DebugScreen(GRY_Game *game) : game(game) {}

DebugScreen::~DebugScreen() { TTF_CloseFont(font); }

/**
* @details
* Loads the font. Cannot be called before GRY_SDL has been initialized.
 */
void DebugScreen::init() {
    GRY_JSON::Document settings;
    GRY_JSON::loadDoc(settings, "assets/debugScreenSettings.json");

    font = game->getSDL().loadFont(settings["fontPath"].GetString(), settings["fontSize"].GetUint());
}

void DebugScreen::process() {
    if (!active) { return; }

    for (auto& text : debugTexts) { text.get()->process(); }

    float y = 0;
    for (auto& text : debugTexts) {
        text.get()->render(0, y);
        y += text->getHeight();
    }
}
