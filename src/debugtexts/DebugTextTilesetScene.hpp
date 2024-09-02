/**
 * @file DebugTextTilesetScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief DebugTextTilesetScene
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "DebugText.hpp"
class TilesetScene;

/**
 * @brief Debug text that displays the number of tiles in a TilesetScene.
 * 
 */
struct DebugTextTilesetScene : public DebugText {
private:
    size_t numTiles = 0;
    TilesetScene* scene;
public:
    DebugTextTilesetScene(TilesetScene* scene);

    void process() final override;
};