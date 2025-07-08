#include "TileMapMenuScene.hpp"
#include "TileMapScene.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"

void Tile::MapMenuScene::makeSelection(uint8_t selection) {
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
	if (!isOpen()) { return; }

	if (readSingleInput() != GCmd::NONE) {
		MapScene* mapScene = (MapScene*)parentScene;
		mapScene->getGame()->getAudio().playSound(mapScene->getSoundResource().sounds.at(1));
	}

	if (subMenu) {
		if (subMenu->isOpen()) {
			renderMenu(font);
			subMenu->process();
			return;
		}
		else { subMenu = nullptr; }
	}

	if (!handleInput()) { return; }

	renderMenu(font);
}

bool Tile::MapMenuScene::load() {
	if (font.path) {
		return
		MenuScene::load() && miscScene.load() &&
		font.load(game);
	}

	GRY_JSON::Document sceneDoc;
	GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the font texture */
	font.setPath(sceneDoc["fontTexturePath"].GetString());
	return false;
}
