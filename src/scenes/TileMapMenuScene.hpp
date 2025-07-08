#pragma once
#include "MenuScene.hpp"
#include "MapMenuMiscScene.hpp"

namespace Tile {
	class MapScene;

	class MapMenuScene : public MenuScene {
	private:
		enum class Selection : uint8_t {
			Item = 0, SpellsSkills = 1,
			Equip = 2, Records = 3,
			Status = 4, Misc = 5
		};

		Fontset font;

		MapMenuMiscScene miscScene;

		MenuScene* subMenu = nullptr;

		void makeSelection(uint8_t selection) final;
	public:
		MapMenuScene(GRY_PixelGame* pGame, const char* path, MapScene* mapScene);

		void init() final;

		void process() final;

		bool load() final;

		const Fontset& getFont() { return font; }
	};
};
