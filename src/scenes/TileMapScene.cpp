#include "TileMapScene.hpp"
#include "GRY_JSON.hpp"
#ifndef NDEBUG
#include "../tile/TileMapImGui.hpp"
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

/**
 * @details
 * Sets up collision rectangles as entities
 */
void TileMapScene::init() {
	GRY_Assert(tileMap.tileLayers.size() >= tileMap.collisionRects.size(),
		"[TileMapScene] Cannot have more object layers than tile layers."
	);
	for (int i = 0; i < tileMap.collisionRects.size(); i++) {
		auto& rectangleLayer = tileMap.collisionRects.at(i);
		auto& tileLayer = tileMap.tileLayers.at(i);
		for (SDL_FRect rect : rectangleLayer) {
			Hitbox hitbox{rect.x, rect.y, rect.w * 0.5f, rect.h * 0.5f};
			hitbox.centerX += hitbox.halfWidth;
			hitbox.centerX += hitbox.halfHeight;
			ECS::entity e = ecs.createEntity();
			ecs.getComponent<Hitbox>().add(e, hitbox);
			unsigned tilex = rect.x / normalTileSize;
			unsigned tiley = rect.y / normalTileSize;
			unsigned tilew = ceilf(rect.w / normalTileSize);
			unsigned tileh = ceilf(rect.h / normalTileSize);
			unsigned startIndex = tiley * tileMap.width + tilex;
			for (int y = 0; y < tileh; y++) {
				for (int x = 0; x < tilew; x++) {
					unsigned index = startIndex + (y * tileMap.width) + x;
					tileLayer.at(index).custom = e;
				}
			}
		}
	}
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
	tileMapImGui(ecs);
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
