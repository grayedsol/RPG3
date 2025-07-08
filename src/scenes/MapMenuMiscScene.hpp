#pragma once
#include "MenuScene.hpp"

namespace Tile {
	class MapMenuScene;

	class MapMenuMiscScene : public MenuScene {
	private:
		enum class Selection : uint8_t {
			Beep = 0, Exit = 1
		};

		void makeSelection(uint8_t selection) final;
	public:
		MapMenuMiscScene(GRY_PixelGame* pGame, const char* path, MapMenuScene* scene);

		void process() final;
	};
};