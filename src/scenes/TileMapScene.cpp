#include "TileMapScene.hpp"
#include "GRY_JSON.hpp"

void TileMapScene::setControls() {
    controls.mapCmd(GCmd::GameMenu, VirtualButton::GAME_A);
    controls.mapCmd(GCmd::GameUp, VirtualButton::GAME_UP);
    controls.mapCmd(GCmd::GameQuit, VirtualButton::GAME_B);
}

void TileMapScene::init() {
	// TODO: Add DebugText
}

void TileMapScene::process() {
	switch (readSingleInput()) {
		case GCmd::GameQuit:
			game->quit();
			break;
		default:
			break;
	}

	tileMapRenderer.process(*tileMap);
}

bool TileMapScene::load() {
	if (tileMap) { return tileMap->load(game); }

    /* Open scene document */
    GRY_JSON::Document sceneDoc;
    GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the tile map */
	tileMap = new TileMap(sceneDoc["tileMapPath"].GetString());
	/* Read the normal tile size */
	normalTileSize = sceneDoc["normalTileSize"].GetUint();

	return false;
}
