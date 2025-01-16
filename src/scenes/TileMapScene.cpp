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
 * Assign collision rectangles to tiles.
 */
void TileMapScene::init() {
	for (int i = 0; i < tileMap.collisionRects.size(); i++) {
		auto& rectangleLayer = tileMap.collisionRects.at(i);
		auto& tileLayer = tileMap.tileLayers.at(i);
		for (int j = 0; j < rectangleLayer.size(); j++) {
			SDL_FRect rect = rectangleLayer.at(j);
			unsigned tilex = rect.x / normalTileSize;
			unsigned tiley = rect.y / normalTileSize;
			unsigned tilew = ceilf(rect.w / normalTileSize);
			unsigned tileh = ceilf(rect.h / normalTileSize);
			unsigned startIndex = tiley * tileMap.width + tilex;
			for (int y = 0; y < tileh; y++) {
				for (int x = 0; x < tilew; x++) {
					unsigned index = startIndex + (y * tileMap.width) + x;
					tileLayer.at(index).collision = j;
				}
			}
		}
	}
	tileMapMovement.init();
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
	tileMapCamera.process();
	tileMapRenderer.process();

	#ifndef NDEBUG
	if (game->debugModeOn()) { tileMapImGui(ecs); }
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

std::vector<SDL_FRect> TileMapScene::queryCollisions(const SDL_FRect rect, std::size_t layer) const {
	auto collides = [rect](const SDL_FRect other) {
		return
			rect.x + rect.w > other.x &&
			rect.x < other.x + other.w &&
			rect.y + rect.h > other.y &&
			rect.y < other.y + other.h;
	};
	std::vector<SDL_FRect> returnVec;
	int tileIndex = (int)(rect.y / normalTileSize) * tileMap.width + (int)(rect.x / normalTileSize);
	int width = (int)ceilf(rect.w / normalTileSize);
	int height = (int)ceilf(rect.h / normalTileSize);

	const TileMap::TileLayer& tileLayer = tileMap.tileLayers.at(layer);
	for (int i = 0; i <= height; i++) {
		for (int j = 0; j <= width; j++) {
			int index = tileIndex + (i * tileMap.width) + j;
			if (index < 0 || index > tileLayer.size()) { continue; }
			Tile::CollisionId collision = tileLayer.at(index).collision;
			if (!collision) { continue; }
			SDL_FRect collisionRect = tileMap.collisionRects.at(layer).at(collision);
			if (collides(collisionRect)) {
				returnVec.push_back(collisionRect);
			}
		}
	}
	return returnVec;
}
