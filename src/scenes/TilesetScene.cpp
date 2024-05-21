/**
 * @file TilesetScene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TilesetScene.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"
#include "../transitions/FadeToBlack.hpp"

/**
 * @details
 * Controls:
 * GAME_A: Quit
 * GAME_B: Switch to a new TilesetScene with transition
*/
void TilesetScene::setControls() {
    controls.mapCmd(GCmd::GameQuit, VirtualButton::GAME_A);
    controls.mapCmd(GCmd::GameMenu, VirtualButton::GAME_B);
}

void TilesetScene::process() {
    /* Check for commands */
    switch (readInput()) {
        case GCmd::GameQuit:
            game->quit();
            break;
        case GCmd::GameMenu:
            game->switchScene(new TilesetScene(game, scenePath), new FadeToBlack(game));
            break;
        default:
            break;
    }

    /* Process tileset animations */
    tileset->processAnimations(game->getDelta());

    /* Tileset width in pixels */
    int tilesetWidth;
    SDL_QueryTexture(tileset->gtexture->texture, NULL, NULL, &tilesetWidth, NULL);
    /* Tileset width in tiles */
    int tilesetWidthTiles = tilesetWidth / tileset->tileWidth;
    /* Destination rectangle for rendering */
    SDL_FRect dstRect{ 0, 0, tileset->tileWidth, tileset->tileHeight };
    /* Display the tileset with a spacing between tiles. Tilesets use 1-based indexing. */
    for (int i = 1; i < tileset->sourceRects.size(); i++) {
        const SDL_FRect* srcRect = tileset->getSourceRect(i);
        SDL_RenderTexture(game->getSDL().getRenderer(), tileset->gtexture->texture, srcRect, &dstRect);

        dstRect.x += tileset->tileWidth + spacing;
        if (dstRect.x >= (spacing + tileset->tileWidth) * tilesetWidthTiles) {
            dstRect.x = 0;
            dstRect.y += tileset->tileHeight + spacing;
        }
    }
}

bool TilesetScene::load() {
    if (tileset) { return tileset->load(game); }
    /* Open scene document */
    GRY_JSON::Document sceneDoc;
    GRY_JSON::loadDoc(sceneDoc, scenePath);

    /* Load the tileset */
    tileset = new Tileset(sceneDoc["tilesetPath"].GetString());
    tileset->load(game);
    return false;
}
