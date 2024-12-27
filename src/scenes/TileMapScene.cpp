#include "TileMapScene.hpp"
#include "GRY_JSON.hpp"
#ifndef NDEBUG
#include "GRY_ECSImGui.hpp"
#endif

void TileMapScene::setControls() {
    controls.mapCmd(GCmd::MapInteract, VirtualButton::GAME_A);
	controls.mapCmd(GCmd::MapSprint, VirtualButton::GAME_LT);
    controls.mapCmd(GCmd::GameQuit, VirtualButton::GAME_B);
	
	controls.mapCmd(GCmd::MapDown, VirtualButton::GAME_DOWN);
	controls.mapCmd(GCmd::MapUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::MapLeft, VirtualButton::GAME_LEFT);
	controls.mapCmd(GCmd::MapRight, VirtualButton::GAME_RIGHT);
}

void TileMapScene::process() {
	switch (readSingleInput()) {
		case GCmd::GameQuit:
			game->quit();
			break;
		default:
			break;
	}
	if (isPressing(GCmd::GameUp)) {}

	tileMapInput.process();
	tileMapMovement.process(game->getDelta());
	tileSpriteAnimator.process(game->getDelta());
	tileMapRenderer.process();

	#ifndef NDEBUG
	imgui_ecs(ecs);
	#endif
}

bool TileMapScene::load() {
	if (tileMap.path && entityMap.path) {
		return tileMap.load(game) && entityMap.load(game);
	}

    /* Open scene document */
    GRY_JSON::Document sceneDoc;
    GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the tile map */
	tileMap.setPath(sceneDoc["tileMapPath"].GetString());
	/* Initialize the tile entity map */
	entityMap.setPath(sceneDoc["tileEntityMapPath"].GetString());
	/* Read the normal tile size */
	normalTileSize = sceneDoc["normalTileSize"].GetUint();

	return false;
}
