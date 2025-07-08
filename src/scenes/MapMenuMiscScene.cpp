#include "MapMenuMiscScene.hpp"
#include "TileMapMenuScene.hpp"
#include "GRY_Game.hpp"

void Tile::MapMenuMiscScene::makeSelection(uint8_t selection) {
	switch (static_cast<Selection>(selection)) {
		case Selection::Beep:
			break;
		case Selection::Exit:
			parentScene->getGame()->quit();
			break;
	}
}

Tile::MapMenuMiscScene::MapMenuMiscScene(GRY_PixelGame* pGame, const char* path, MapMenuScene* scene) :
	MenuScene(pGame, path, (Scene*)scene) {
}

void Tile::MapMenuMiscScene::process() {
	if (!isOpen()) { return; }

	if (!handleInput()) { return; }

	renderMenu(((MapMenuScene*)parentScene)->getFont());
}
