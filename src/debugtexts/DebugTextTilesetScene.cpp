#include "DebugTextTilesetScene.hpp"
#include "../scenes/TilesetScene.hpp"

DebugTextTilesetScene::DebugTextTilesetScene(TilesetScene *scene) : 
    DebugText(&scene->getGame()->getDebugScreen()), scene(scene) {
}

void DebugTextTilesetScene::process() {
    size_t newNumTiles = scene->tileset->sourceRects.size() - 1;

    /* Change texture only if number of tiles has changed */
    if (numTiles != newNumTiles) {
        /* Format newNumTiles to C string */
        char output[32];
        snprintf(output, 16, "Tile count: %i", newNumTiles);

        /* Update texture */
        updateText(output);

        numTiles = newNumTiles;
    }
}
