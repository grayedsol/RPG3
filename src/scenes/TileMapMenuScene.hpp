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

		MapMenuMiscScene miscScene;

		void makeSelection() final;
	public:
		MapMenuScene(GRY_PixelGame* pGame, const char* path, MapScene* mapScene);

		void init() final;

		void process() final;

		bool load() final;
	};
};
