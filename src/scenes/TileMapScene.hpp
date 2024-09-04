/**
 * @file TileMapScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TileMapScene
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Scene.hpp"
#include "TileMap.hpp"

/**
 * @brief Loads a TileMap.
 * 
 */
class TileMapScene : public Scene {
	// friend class DebugTextTileMapScene
private:
	/**
	 * @brief TileMap that will be loaded.
	 * 
	 */
	TileMap* tileMap = nullptr;

	/**
	 * @brief @copybrief Scene::setControls
	 * 
	 */
	void setControls() final override;
public:
    /**
     * @brief Constructor.
     * 
     * @param game Associated game class.
     * @param tileMapPath File path to the tilemap scene.
     */
	TileMapScene(GRY_Game* game, const char* tileMapPath) : Scene(game, tileMapPath) {}

	~TileMapScene() { delete tileMap; tileMap = nullptr; }

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
