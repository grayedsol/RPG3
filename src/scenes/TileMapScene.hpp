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
#include "DialogueResource.hpp"
#include "../tile/TileMapScriptResource.hpp"

class GRY_PixelGame;

namespace Tile {
	struct MapSceneInfo {
		Position2 spawnPosition = Position2{ -1, -1 };
		Direction spawnDirection = Direction::DirectionNone;
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
	private:
		/**
		 * @brief TileMap ECS structure.
		 *
		 */
		MapECS ecs;

		/**
		 * @brief TileMap that will be loaded.
		 *
		 */
		TileMap tileMap;

		/**
		 * @brief TileEntityMap that will be loaded.
		 *
		 */
		EntityMap entityMap;

		/**
		 * @brief Container for dialogue data.
		 * 
		 */
		DialogueResource mapDialogues;

		/**
		 * @brief Container for script data.
		 * 
		 */
		MapScriptResource mapScripts;

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
		 * @brief Stores quadtrees for the collision hitboxes of entities for each layer.
		 * 
		 */
		MapQuadTrees tileMapQuadTrees;

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
		 * @brief Text box scene.
		 * 
		 */
		TextBoxScene textBoxScene;

		MapSpeak tileMapSpeak;

		MapScripting mapScripting;

		MapSceneInfo sceneInfo;

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
		MapScene(GRY_PixelGame *pGame, const char *tileMapPath, MapSceneInfo sceneInfo = MapSceneInfo{}) :
			Scene((GRY_Game *)pGame, tileMapPath),
			entityMap(ecs),
			tileMapRenderer(this),
			tileMapCamera(this),
			tileMapMovement(this),
			tileMapQuadTrees(this),
			tileSpriteAnimator(this),
			tileMapInput(this),
			textBoxScene(pGame, "assets/textboxscene/scene.json", this),
			tileMapSpeak(this),
			mapScripting(this),
			sceneInfo(sceneInfo) {
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
		MapECS& getECS() { return ecs; }

		/**
		 * @copybrief getECS
		 *
		 * @return `const` reference to the TileMapECS.
		 */
		const MapECS& getECSReadOnly() const { return ecs; }

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
		const EntityMap& getTileEntityMap() const { return entityMap; }

		/**
		 * @brief Get a reference to the TileEntityMap.
		 *
		 * @return Reference to the TileEntityMap.
		 */
		EntityMap& getTileEntityMap() { return entityMap; }

		TextBoxScene& getTextBox() { return textBoxScene; }

		MapSpeak& getTileMapSpeak() { return tileMapSpeak; }

		const DialogueResource& getDialogueResource() { return mapDialogues; }

		const MapScriptResource& getScriptResource() { return mapScripts; }

		/**
		 * @brief Get the width / height of a normal square tile, in pixels.
		 *
		 * @copydetails MapScene::normalTileSize
		 *
		 * @return The normal tile size in pixels.
		 */
		uint16_t getNormalTileSize() const { return normalTileSize; }

		/**
		 * @copydoc TileMapRenderer::setOffset
		 */
		void setRenderOffset(float x, float y) {
			tileMapRenderer.setOffset(x,y);
		}

		const std::vector<QuadTree>& getQuadTrees() { return tileMapQuadTrees.getQuadTrees(); }

		const std::vector<QuadTree>& getSoftQuadTrees() { return tileMapQuadTrees.getSoftQuadTrees(); }

		void updateQuadTree(Hitbox oldBox, Hitbox newBox, ECS::entity e, unsigned layer) {
			tileMapQuadTrees.updateQuadTree(oldBox, newBox, e, layer);
		}
		
		/**
		 * @brief Get a vector of collision rectangles that collide with `rect`.
		 * 
		 * @param rect SDL_FRect to test collision against
		 * @param layer Which collision layer to test within
		 * @return Vector of rectangles colliding with `rect`
		 */
		std::vector<SDL_FRect> queryTileCollisions(const SDL_FRect rect, std::size_t layer) const;

		bool executeCommand(MapCommand& command);

		void switchMap(const char* mapScenePath, MapSceneInfo sceneInfo = MapSceneInfo{});
	};
};
