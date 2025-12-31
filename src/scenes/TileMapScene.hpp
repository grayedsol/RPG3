/**
 * @file TileMapScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapScene
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "../tile/TileMapMovement.hpp"
#include "../tile/TileMapQuadTrees.hpp"
#include "../tile/TileSpriteAnimator.hpp"
#include "../tile/TileMapRenderer.hpp"
#include "../tile/TileMapCamera.hpp"
#include "Scene.hpp"
#include "../tile/TileMapECS.hpp"
#include "../tile/TileMapInput.hpp"
#include "../tile/TileMapSpeak.hpp"
#include "../tile/TileMapScripting.hpp"
#include "TextBoxScene.hpp"
#include "../tile/TileMapDialogueResource.hpp"
#include "../tile/TileMapScriptResource.hpp"
#include "TileMapMenuScene.hpp"
#include "SoundResource.hpp"

class GRY_PixelGame;

namespace Tile {
	struct MapSceneInfo {
		Position2 spawnPosition = Position2{ -1, -1 };
		Direction spawnDirection = Direction::DirectionNone;
	};

	/**
	 * @brief Rendering offset.
	 * 
	 */
	struct MapRenderOffset {
		float x;
		float y;
	};
	/**
	 * @brief Loads a TileMap and TileEntityMap into a playable scene.
	 * 
	 * @details
	 * Controls:
	 * Directional inputs: Move
	 * GAME_A: Interact
	 * GAME_B: Sprint
	 * GAME_LT: Quit
	 */
	class MapScene : public Scene {
		friend class MapScripting;
	private:
		/**
		 * @brief Tile Map ECS structure.
		 *
		 */
		MapECS ecs;

		/**
		 * @brief Tile::Map that will be loaded.
		 *
		 */
		TileMap tileMap;

		/**
		 * @brief Tile::EntityMap that will be loaded.
		 *
		 */
		EntityMap entityMap;

		/**
		 * @brief Container for dialogue data.
		 * 
		 */
		MapDialogueResource mapDialogues;

		/**
		 * @brief Container for script data.
		 * 
		 */
		MapScriptResource mapScripts;

		/**
		 * @brief Container for sounds.
		 * 
		 */
		SoundResource sounds;

		/**
		 * @brief Stores quadtrees for the collision hitboxes of entities for each layer.
		 * 
		 */
		MapQuadTrees tileMapQuadTrees;

		/**
		 * @copybrief MapRenderOffset
		 * 
		 */
		MapRenderOffset renderOffset;

		/**
		 * @brief Renderer for the tile map.
		 *
		 */
		MapRenderer tileMapRenderer;
		
		/**
		 * @brief Camera controller.
		 * 
		 */
		MapCamera tileMapCamera;

		/**
		 * @brief Movement for the tile map entities.
		 *
		 */
		MapMovement tileMapMovement;

		/**
		 * @brief Input system for the tile map.
		 * 
		 */
		MapInput tileMapInput;

		/**
		 * @brief Animator for sprites.
		 * 
		 */
		SpriteAnimator tileSpriteAnimator;

		/**
		 * @copybrief TextBoxScene
		 * 
		 */
		TextBoxScene textBoxScene;

		/**
		 * @copybrief MapMenuScene
		 * 
		 */
		MapMenuScene menuScene;

		MapSpeak tileMapSpeak;

		MapScripting mapScripting;

		MapSceneInfo sceneInfo;

		/**
		 * @brief Width and height of a normal square tile, in pixels.
		 *
		 * @details
		 * Normal tiles should have a square shape, typically 8x8, 16x16, etc.
		 * Things such as character sprites may be rectangles, e.g., a 16x32 sprite,
		 * and this standard tile size is needed to interpret the size of the sprite
		 * in terms of normal tiles.
		 */
		uint16_t normalTileSize = 0;

		/**
		 * @copybrief Scene::setControls
		 *
		 */
		void setControls() final;

		void switchMap(const char* mapScenePath, MapSceneInfo sceneInfo = MapSceneInfo{});
	public:
		/**
		 * @brief Constructor.
		 *
		 * @param game Associated game class.
		 * @param tileMapPath File path to the tilemap scene.
		 */
		MapScene(GRY_PixelGame *pGame, const char *tileMapPath, MapSceneInfo sceneInfo = MapSceneInfo{});

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
		 * @brief Sets the controls to normal while retaining the control scheme's activation state.
		 * 
		 * @sa disablePlayerControls
		 */
		void enablePlayerControls();

		/**
		 * @brief Sets the controls to do nothing while retaining the control scheme's activation state.
		 * 
		 * @sa enablePlayerControls
		 */
		void disablePlayerControls();

		/**
		 * @copydoc MapScripting::executeCommand
		 *  
		 */
		bool executeCommand(MapCommand& command);

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
		MapECS& getECS() { return ecs; }

		/**
		 * @copybrief getECS
		 *
		 * @return `const` reference to the TileMapECS.
		 */
		const MapECS& getECSReadOnly() const { return ecs; }

		/**
		 * @brief Get the width / height of a normal square tile, in pixels.
		 *
		 * @copydetails MapScene::normalTileSize
		 *
		 * @return The normal tile size in pixels.
		 */
		uint16_t getNormalTileSize() const { return normalTileSize; }
		
		/**
		 * @brief Get a vector of collision rectangles that collide with `rect`.
		 * 
		 * @param rect SDL_FRect to test collision against
		 * @param layer Which collision layer to test within
		 * @return Vector of rectangles colliding with `rect`
		 */
		std::vector<SDL_FRect> queryTileCollisions(const SDL_FRect rect, std::size_t layer) const;
	};
};
