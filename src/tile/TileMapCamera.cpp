/**
 * @file TileMapCamera.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TileMapCamera.hpp"
#include "../scenes/TileMapScene.hpp"
#include "GRY_PixelGame.hpp"

Tile::MapCamera::MapCamera(MapScene *scene) : scene(scene),
	hitboxes(&scene->getECSReadOnly().getComponentReadOnly<Hitbox>()),
	players(&scene->getECSReadOnly().getComponentReadOnly<Player>()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void Tile::MapCamera::process() {
	GRY_Assert(players->size() > 0, "[TileMapCamera] There were no players.");
	ECS::entity e = players->getEntity(0);
	GRY_PixelGame* game = scene->getPixelGame();
	float playerX = hitboxes->get(e).x + (hitboxes->get(e).w * 0.5f);
	float playerY = hitboxes->get(e).y + (hitboxes->get(e).h * 0.5f);
	scene->setRenderOffset(
		(game->getScreenWidthPixels() * 0.5f) - playerX,
		(game->getScreenHeightPixels() * 0.5f) - playerY
	);
}
