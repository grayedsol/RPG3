/**
 * @file TileMapScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TileMapScene
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "../tile/TileMapMovement.hpp"
#include "../tile/TileSpriteAnimator.hpp"
#include "../tile/TileMapRenderer.hpp"
#include "../tile/TileMapCamera.hpp"
#include "Scene.hpp"
#include "../tile/TileMapECS.hpp"
#include "../tile/TileMapInput.hpp"
#include "TextBoxScene.hpp"

class GRY_PixelGame;

/**
 * @brief Loads a TileMap and TileEntityMap into a playable scene.
 * 
 * @details
 * Controls:
 * Directional inputs: Move
 * GAME_A: Interact
 * GAME_LT: Sprint
 * GAME_B: Quit
 */
class TileMapScene : public Scene {
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
	TileMap tileMap;

	/**
	 * @brief TileEntityMap that will be loaded.
	 *
	 */
	TileEntityMap entityMap;

	/**
	 * @brief Renderer for the tile map.
	 *
	 */
	TileMapRenderer tileMapRenderer;
	
	/**
	 * @brief Camera controller.
	 * 
	 */
	TileMapCamera tileMapCamera;

	/**
	 * @brief Movement for the tile map entities.
	 *
	 */
	TileMapMovement tileMapMovement;

	/**
	 * @brief Input system for the tile map.
	 * 
	 */
	TileMapInput tileMapInput;

	/**
	 * @brief Animator for sprites.
	 * 
	 */
	TileSpriteAnimator tileSpriteAnimator;

	/**
	 * @brief Text box scene.
	 * 
	 */
	TextBoxScene textBoxScene;

	/**
	 * @brief Width and height of a normal square tile, in pixels.
	 *
	 * @details
	 * Normal tiles should have a square shape, typically 8x8, 16x16, etc.
	 * Things such as character sprites may be rectangles, e.g., a 16x32 sprite,
	 * and this a standard tile size is needed to interpret the size of the sprite
	 * in terms of normal tiles.
	 */
	uint16_t normalTileSize = 0;

	/**
	 * @copybrief Scene::setControls
	 *
	 */
	void setControls() final;
public:
	/**
	 * @brief Constructor.
	 *
	 * @param game Associated game class.
	 * @param tileMapPath File path to the tilemap scene.
	 */
	TileMapScene(GRY_PixelGame *pGame, const char *tileMapPath) :
		Scene((GRY_Game *)pGame, tileMapPath),
		entityMap(ecs),
		tileMapRenderer(this),
		tileMapCamera(this),
		tileMapMovement(this),
		tileSpriteAnimator(this),
		tileMapInput(this),
		textBoxScene(pGame, "assets/textboxscene/scene.json", this) {
	}

	/**
	 * @brief Initializes the scene.
	 *
	 */
	void init() final;

	/**
	 * @copydoc Scene::process
	 */
	void process() final;

	/**
	 * @copydoc Scene::load
	 */
	bool load() final;

	/**
	 * @brief Get a pointer to the GRY_PixelGame.
	 *
	 * @return Pointer to the GRY_PixelGame.
	 */
	GRY_PixelGame* getPixelGame() const { return (GRY_PixelGame*)game; }

	/**
	 * @brief Get a reference to the TileMapECS.
	 *
	 * @return Reference to the TileMapECS.
	 */
	TileMapECS& getECS() { return ecs; }

	/**
	 * @copybrief getECS
	 *
	 * @return `const` reference to the TileMapECS.
	 */
	const TileMapECS& getECSReadOnly() const { return ecs; }

	/**
	 * @brief Get a reference to the TileMap.
	 *
	 * @return `const` reference to the TileMap.
	 */
	const TileMap& getTileMap() const { return tileMap; }

	/**
	 * @brief Get a reference to the TileEntityMap.
	 *
	 * @return `const` reference to the TileEntityMap.
	 */
	const TileEntityMap& getTileEntityMap() const { return entityMap; }

	/**
	 * @brief Get a reference to the TileEntityMap.
	 *
	 * @return Reference to the TileEntityMap.
	 */
	TileEntityMap& getTileEntityMap() { return entityMap; }

	/**
	 * @brief Get the width / height of a normal square tile, in pixels.
	 *
	 * @copydetails TileMapScene::normalTileSize
	 *
	 * @return The normal tile size in pixels.
	 */
	uint16_t getNormalTileSize() const { return normalTileSize; }

	/**
	 * @copydoc TileMapRenderer::setOffset
	 * 
	 */
	void setRenderOffset(float x, float y) {
		tileMapRenderer.setOffset(x,y);
	}

	/**
	 * @brief Get a vector of collision rectangles that collide with `rect`.
	 * 
	 * @param rect SDL_FRect to test collision against
	 * @param layer Which collision layer to test within
	 * @return Vector of rectangles colliding with `rect`
	 */
	std::vector<SDL_FRect> queryCollisions(const SDL_FRect rect, std::size_t layer) const;
};
