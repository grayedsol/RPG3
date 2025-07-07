#include "MapMenuMiscScene.hpp"
#include "TileMapMenuScene.hpp"
#include "GRY_Game.hpp"

void Tile::MapMenuMiscScene::makeSelection() {
	switch (static_cast<Selection>(selection)) {
		case Selection::Beep:
			break;
		case Selection::Exit:
			scene->getGame()->quit();
			break;
	}
}

Tile::MapMenuMiscScene::MapMenuMiscScene(GRY_PixelGame* pGame, const char* path, MapMenuScene* scene) :
	MenuScene(pGame, path, (Scene*)scene) {
}