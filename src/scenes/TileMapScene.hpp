/**
 * @file TileMapScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TileMapScene
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Scene.hpp"
#include "TileMapECS.hpp"
#include "../tile/TileMapRenderer.hpp"

class GRY_PixelGame;

/**
 * @brief Loads a TileMap.
 * 
 */
class TileMapScene : public Scene {
	/* friend class DebugTextTileMapScene */
private:
	/**
	 * @brief TileMap ECS structure.
	 * 
	 */
	TileMapECS ecs;

	/**
	 * @brief TileMap that will be loaded.
	 * 
	 */
	TileMap* tileMap = nullptr;

	/**
	 * @brief TileEntityMap that will be loaded.
	 * 
	 */
	TileEntityMap* entityMap = nullptr;

	/**
	 * @brief Width and height of a normal square tile, in pixels.
	 * 
	 * @details
	 * Normal tiles should have a square shape, typically 8x8, 16x16, etc.
	 * Things such as character sprites may be rectangles, e.g., a 16x32 sprite,
	 * and so a standard tile size is needed to interpret the size of the sprite
	 * in terms of normal tiles.
	 */
	uint16_t normalTileSize = 0;

	/**
	 * @brief Renderer for the tile map.
	 * 
	 */
	TileMapRenderer tileMapRenderer;

	/**
	 * @copybrief Scene::setControls
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
	TileMapScene(GRY_PixelGame* pGame, const char* tileMapPath) : Scene((GRY_Game*)pGame, tileMapPath), tileMapRenderer(this) {}

	~TileMapScene() { delete tileMap; tileMap = nullptr; }

    /**
     * @brief Initializes the scene.
     * 
     */
    void init() final override;

    /**
     * @copydoc Scene::process
     */
    void process() final override;

    /**
     * @copydoc Scene::load
     */
    bool load() final override;

	/**
	 * @brief Get a pointer to the GRY_PixelGame.
	 * 
	 * @return Pointer to the GRY_PixelGame.
	 */
	GRY_PixelGame* getPixelGame() const { return (GRY_PixelGame*)game; }

	/**
	 * @brief Get the width / height of a normal square tile.
	 * 
	 * @copydetails TileMapScene::normalTileSize
	 * 
	 * @return The normal tile size.
	 */
	uint16_t getNormalTileSize() const { return normalTileSize; }
};
