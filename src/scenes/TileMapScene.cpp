#include "TileMapScene.hpp"
#include "GRY_PixelGame.hpp"
#include "GRY_JSON.hpp"
#include "../transitions/FadeToBlack.hpp"
#ifndef NDEBUG
#include "../tile/TileMapImGui.hpp"
#endif

void Tile::MapScene::setControls() {
    controls.mapCmd(GCmd::MapInteract, VirtualButton::GAME_A);
	controls.mapCmd(GCmd::MapSprint, VirtualButton::GAME_B);
	controls.mapCmd(GCmd::MapMenu, VirtualButton::GAME_X);
    controls.mapCmd(GCmd::GameQuit, VirtualButton::GAME_LT);
	
	controls.mapCmd(GCmd::MapDown, VirtualButton::GAME_DOWN);
	controls.mapCmd(GCmd::MapUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::MapLeft, VirtualButton::GAME_LEFT);
	controls.mapCmd(GCmd::MapRight, VirtualButton::GAME_RIGHT);
}

Tile::MapScene::MapScene(GRY_PixelGame *pGame, const char *tileMapPath, MapSceneInfo sceneInfo) :
	Scene((GRY_Game *)pGame, tileMapPath),
	entityMap(ecs),
	tileMapRenderer(this),
	tileMapCamera(this),
	tileMapMovement(this),
	tileMapQuadTrees(this),
	tileSpriteAnimator(this),
	tileMapInput(this),
	textBoxScene(pGame, "assets/textboxscene/scene.json", this),
	tileMapSpeak(this),
	mapScripting(this),
	menuScene(pGame, "assets/mapmenuscene/scene.json", this),
	sceneInfo(sceneInfo) {
}

/**
 * @details
 * Assign collision rectangles to tiles and initialize systems.
 */
void Tile::MapScene::init() {
	setControls();
	
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

	tileMapQuadTrees.init();
	textBoxScene.init();
	menuScene.init();

	entity player = ecs.getComponent<Player>().getEntity(0);
	if (sceneInfo.spawnPosition.x >= 0 && sceneInfo.spawnPosition.y >= 0) {
		ecs.getComponent<Position2>().get(player) = sceneInfo.spawnPosition;
	}
	if (sceneInfo.spawnDirection != Direction::DirectionNone) {
		ecs.getComponent<Actor>().get(player).direction = sceneInfo.spawnDirection;
	}
}

void Tile::MapScene::process() {
	GCmd cmd = readSingleInput();
	switch (cmd) {
		case GCmd::GameQuit:
			game->quit();
			break;
		case GCmd::MapMenu:
			menuScene.open();
			break;
		default:
			break;
	}

	tileMapInput.process();
	mapScripting.process(game->getDelta());
	tileMapSpeak.process();
	tileMapMovement.process(game->getDelta());
	tileMapQuadTrees.process();
	tileSpriteAnimator.process(game->getDelta());
	tileMapCamera.process();
	tileMap.tileset.processAnimations(game->getDelta());
	tileMapRenderer.process();
	
	tileMapMovement.postProcess();
	
	textBoxScene.process();
	menuScene.process();

	EntityMap::updateLayers(&entityMap);

	#ifndef NDEBUG
	if (game->debugMenuIsOn()) { tileMapImGui(ecs); }
	#endif
}

bool Tile::MapScene::load() {
	if (tileMap.path && entityMap.path && mapDialogues.path && sounds.path && mapScripts.path) {
		return
		tileMap.load(game) && entityMap.load(game) &&
		mapDialogues.load(game) && textBoxScene.load() &&
		menuScene.load() && sounds.load(game) &&
		mapScripts.load(game);
	}

    /* Open scene document */
    GRY_JSON::Document sceneDoc;
    GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the tile map */
	tileMap.setPath(sceneDoc["tileMapPath"].GetString());
	/* Initialize the tile entity map */
	entityMap.setPath(sceneDoc["tileEntityMapPath"].GetString());
	/* Initialize the dialogue resource */
	mapDialogues.setPath(sceneDoc["dialoguePath"].GetString());
	/* Initialize the script resource */
	mapScripts.setPath(sceneDoc["scriptsPath"].GetString());
	/* Initialize the sound resource */
	sounds.setPath(sceneDoc["soundsPath"].GetString());
	/* Read the normal tile size */
	normalTileSize = sceneDoc["normalTileSize"].GetUint();

	return false;
}

void Tile::MapScene::activateControls() {
	setControls();
	game->setControlScheme(controls);
}

void Tile::MapScene::deactivateControls() {
	controls.resetCmds();
	game->setControlScheme(controls);
}

std::vector<SDL_FRect> Tile::MapScene::queryTileCollisions(const SDL_FRect rect, std::size_t layer) const {
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

	const TileLayer& tileLayer = tileMap.tileLayers.at(layer);
	for (int i = 0; i <= height; i++) {
		for (int j = 0; j <= width; j++) {
			int index = tileIndex + (i * tileMap.width) + j;
			if (index < 0 || index > tileLayer.size()) { continue; }
			CollisionId collision = tileLayer.at(index).collision;
			if (!collision) { continue; }
			SDL_FRect collisionRect = tileMap.collisionRects.at(layer).at(collision);
			if (collides(collisionRect)) {
				returnVec.push_back(collisionRect);
			}
		}
	}
	return returnVec;
}

bool Tile::MapScene::executeCommand(MapCommand &command) {
	return mapScripting.executeCommand(command, game->getDelta());
}

void Tile::MapScene::switchMap(const char *mapScenePath, MapSceneInfo sceneInfo) {
	MapScene* newMapScene = new MapScene((GRY_PixelGame*)game, mapScenePath, sceneInfo);
	game->switchScene(newMapScene, new FadeToBlack(game));
}
