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
	positions(&scene->getECSReadOnly().getComponentReadOnly<Position2>()),
	players(&scene->getECSReadOnly().getComponentReadOnly<Player>()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void Tile::MapCamera::process() {
	GRY_Assert(players->size() > 0, "[TileMapCamera] There were no players.");
	
	if (cameraLocked) {
		Hitbox playerHitbox = hitboxes->get(players->getEntity(0));
		center = Position2 {
			playerHitbox.x + (playerHitbox.w * 0.5f) - .01f, /**< Subtracting .01f takes care of some floating point errors */
			playerHitbox.y + (playerHitbox.h * 0.5f) - .01f
		};
	}

	scene->setRenderOffset(
		(scene->getPixelGame()->getScreenWidthPixels() * 0.5f) - center.x,
		(scene->getPixelGame()->getScreenHeightPixels() * 0.5f) - center.y
	);
}

bool Tile::MapCamera::moveCamera(Position2 position, double speed, double delta) {
	if (cameraLocked) { GRY_Log("Tried to move the camera while it was locked.\n"); }

	Velocity2 moveVec = position - center;
	float distance = GRY_VecLengthSq(moveVec);
	if (distance == 0) { return true; }
	moveVec = GRY_VecNormalize(moveVec);
	moveVec *= delta * speed;
	if (distance <= GRY_VecLengthSq(moveVec)) {
		center = position;
		return true;
	}
	center += moveVec;
	return false;
}

bool Tile::MapCamera::moveCameraToPlayer(double speed, double delta) {
	Hitbox playerHitbox = hitboxes->get(players->getEntity(0));
	Position2 target = Position2 {
		playerHitbox.x + (playerHitbox.w * 0.5f) - .01f, /**< Subtracting .01f takes care of some floating point errors */
		playerHitbox.y + (playerHitbox.h * 0.5f) - .01f
	};
	return moveCamera(target, speed, delta);
}
