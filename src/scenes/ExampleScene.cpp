/**
 * @file ExampleScene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "ExampleScene.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"
#include "SDL3/SDL_render.h"
#include "../transitions/FadeToBlack.hpp"

/**
 * @details
 * Controls:
 * GAME_A: Quit
 * GAME_B: Switch to a new ExampleScene with transition
*/
void ExampleScene::setControls() {
    controls.mapCmd(GCmd::GameMenu, VirtualButton::GAME_A);
    controls.mapCmd(GCmd::GameQuit, VirtualButton::GAME_B);
}

void ExampleScene::process() {
    /* Check for inputs from this frame */
    switch (readSingleInput()) {
        case GCmd::GameMenu:
            game->switchScene(new ExampleScene(game, scenePath), new FadeToBlack(game));
            break;
        case GCmd::GameQuit:
            game->quit();
            break;
        default:
            break;
    }

    /* Fill the window with the texture */
    int windowWidth, windowHeight;
    game->getSDL().getWindowSize(&windowWidth, &windowHeight);
    SDL_FRect dstRect { 0, 0, (float)windowWidth, (float)windowHeight };
    SDL_RenderTexture(game->getSDL().getRenderer(), examplePng.texture, NULL, &dstRect);
}

bool ExampleScene::load() {
    if (examplePng.path) { return examplePng.load(game); }
	
    /* Open scene document */
    GRY_JSON::Document sceneDoc;
    GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the examplePng */
	examplePng.setPath(sceneDoc["texturePath"].GetString());
    
    return false;
}
