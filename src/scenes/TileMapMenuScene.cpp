#include "TileMapMenuScene.hpp"

void Tile::MapMenuScene::makeSelection() {
	switch (static_cast<Selection>(selection)) {
		case Selection::Item:
			break;
		case Selection::SpellsSkills:
			break;
		case Selection::Equip:
			break;
		case Selection::Records:
			break;
		case Selection::Status:
			break;
		case Selection::Misc:
			subMenu = &miscScene;
			miscScene.open();
			break;
	}
}

Tile::MapMenuScene::MapMenuScene(GRY_PixelGame* pGame, const char* path, MapScene* mapScene) :
	MenuScene(pGame, path, (Scene*)mapScene),
	miscScene(pGame, "assets/mapmenuscene/misc/scene.json", this) {
}

void Tile::MapMenuScene::init() {
	MenuScene::init();
	miscScene.init();
}

void Tile::MapMenuScene::process() {
	MenuScene::process();
	miscScene.process();
}

bool Tile::MapMenuScene::load() {
	return MenuScene::load() && miscScene.load();
}
