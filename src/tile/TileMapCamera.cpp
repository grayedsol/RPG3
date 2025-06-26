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
	
	Hitbox playerHitbox = hitboxes->get(players->getEntity(0));

	center = Position2 {
		playerHitbox.x + (playerHitbox.w * 0.5f) - .01f, /**< Subtracting .01f takes care of some floating point errors */
		playerHitbox.y + (playerHitbox.h * 0.5f) - .01f
	};

	scene->setRenderOffset(
		(scene->getPixelGame()->getScreenWidthPixels() * 0.5f) - center.x,
		(scene->getPixelGame()->getScreenHeightPixels() * 0.5f) - center.y
	);
}
