/**
 * @file ExampleScene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "ExampleScene.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"
#include "../transitions/FadeToBlack.hpp"

/**
 * @details
 * Controls:
 * GAME_A: Quit
 * GAME_B: Switch to a new ExampleScene with transition
*/
void ExampleScene::setControls() {
    controls.mapCmd(GCmd::GameQuit, VirtualButton::GAME_A);
    controls.mapCmd(GCmd::GameMenu, VirtualButton::GAME_B);
}

void ExampleScene::process() {
    /* Check for inputs from this frame */
    switch (readSingleInput()) {
        case GCmd::GameQuit:
            game->quit();
            break;
        case GCmd::GameMenu:
            game->switchScene(new ExampleScene(game, scenePath), new FadeToBlack(game));
            break;
        default:
            break;
    }

    /* Fill the window with the texture */
    int windowWidth, windowHeight;
    game->getSDL().getWindowSize(&windowWidth, &windowHeight);
    SDL_FRect dstRect { 0, 0, (float)windowWidth, (float)windowHeight };
    SDL_RenderTexture(game->getSDL().getRenderer(), examplePng.get()->texture, NULL, &dstRect);
}

bool ExampleScene::load() {
    if (examplePng.get()) { return true; }
    /* Open scene document */
    GRY_JSON::Document sceneDoc;
    GRY_JSON::loadDoc(sceneDoc, scenePath);

    /* Load the texture */
    examplePng = std::make_unique<GRY_Texture>(sceneDoc["texturePath"].GetString());
    examplePng.get()->load(game);
    return false;
}
