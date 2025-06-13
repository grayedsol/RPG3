/**
 * @file TilesetScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::TilesetScene
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Scene.hpp"
#include "../tile/Tileset.hpp"
#include "../tile/TileCollision.hpp"

struct SDL_Renderer;

namespace Tile {
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
		Tileset tileset;

		/**
		 * @brief Collision data for each tile in the tileset.
		 * 
		 */
		Collision collisions;

		/**
		 * @brief Pointer to the renderer.
		 * 
		 */
		SDL_Renderer* renderer = nullptr;

		/**
		 * @brief Distance between each displayed tile, in pixels.
		*/
		unsigned int spacing = 2;

		/**
		 * @copybrief Scene::setControls
		 */
		void setControls() final;
	public:
		/**
		 * @brief Constructor.
		 * 
		 * @param game Associated game class.
		 * @param tilesetPath File path to the tileset scene.
		 */
		TilesetScene(GRY_Game* game, const char* tilesetPath);
		
		/**
		 * @brief Initializes the scene.
		 * 
		 */
		void init() final { setControls(); }

		/**
		 * @copydoc Scene::process
		 */
		void process();

		/**
		 * @copydoc Scene::load
		 */
		bool load() final;
	};
};
