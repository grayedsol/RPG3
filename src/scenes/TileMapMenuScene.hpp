/**
 * @file TileMapMenuScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapMenuScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "GRY_Texture.hpp"
#include "../textbox/Fontset.hpp"

class GRY_PixelGame;
struct SDL_Renderer;

namespace Tile {
	class MapScene;

	class MapMenuScene : public Scene {
	private:
		MapScene* scene;

		enum class Selection : int8_t {
			None = 0,
			Item = 1,
			SpellsSkills = 2,
			Equip = 3,
			Records = 4,
			Status = 5,
			Misc = 6,
			Size = 7
		} selection = Selection::Item;

		char selectionStrings[static_cast<int8_t>(Selection::Size)][18] = {
			"",
			">Item", " Spells & Skills",
			" Equip", " Records",
			" Status", " Misc."
		};

		GRY_Texture boxTexture;

		SDL_FRect boxTextureArea;

		SDL_FRect textArea;

		SDL_Renderer* renderer;

		const float* pixelScaling;

		bool active = false;

		Fontset font;

		/**
		 * @copybrief Scene::setControls
		 * 
		 */
		void setControls();

		void setSelection(Selection selectionValue);

		void renderMenu();
	public:
		MapMenuScene(GRY_PixelGame* pGame, const char* path, MapScene* scene);

		/**
		 * @brief Initializes the scene.
		 *
		 */
		void init() final;

		/**
		 * @copydoc Scene::process
		 * 
		 */
		void process() final;

		/**
		 * @copydoc Scene::load
		 */
		bool load() final;

		void open();

		void close();
	};
};