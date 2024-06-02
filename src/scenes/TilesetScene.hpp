/**
 * @file TilesetScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TilesetScene
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Scene.hpp"
#include "Tileset.hpp"
#include "TileCollision.hpp"

/**
 * @brief Loads and displays a tileset.
 * 
 * @details
 * The scene displays a tileset from the assets folder on the screen.
 * Useful for testing purposes.
 * 
 * The player can press GAME_A to switch to a new instance
 * of this scene with a FadeToBlack transition, or GAME_B to exit.
 * Pressing GAME_UP will highlight the collision boxes for the tiles.
 */
class TilesetScene : public Scene {
    friend class DebugTextTilesetScene;
private:
    /**
     * @brief Tileset that will be loaded and displayed.
     * 
    */
    Tileset* tileset = nullptr;

    /**
     * @brief Collision data for each tile in the tileset.
     * 
     */
    TileCollision* collisions = nullptr;

    /**
     * @brief Distance between each displayed tile, in pixels.
    */
    const unsigned int spacing = 2;

    /**
     * @brief @copybrief Scene::setControls
     */
    void setControls() final override;
public:
    /**
     * @brief Constructor.
     * 
     * @param game Associated game class.
     * @param path File path to the tileset.
     */
    TilesetScene(GRY_Game* game, const char* tilesetPath) : Scene(game, tilesetPath) {}

    ~TilesetScene() {
        delete tileset; tileset = nullptr;
        delete collisions; collisions = nullptr;
    }

    /**
     * @brief Initializes the scene.
     * 
     */
    void init() final override;

    /**
     * @copydoc Scene::process
     */
    void process();

    /**
     * @copydoc Scene::load
     */
    bool load() final override;
};
