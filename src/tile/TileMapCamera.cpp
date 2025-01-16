#include "TileMapCamera.hpp"
#include "../scenes/TileMapScene.hpp"
#include "GRY_PixelGame.hpp"

TileMapCamera::TileMapCamera(TileMapScene *scene) : scene(scene),
	hitboxes(&scene->getECSReadOnly().getComponentReadOnly<Hitbox>()),
	players(&scene->getECSReadOnly().getComponentReadOnly<Player>()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void TileMapCamera::process() {
	GRY_Assert(players->size() > 0, "[TileMapCamera] There were no players.");
	GRY_PixelGame* game = scene->getPixelGame();
	ECS::entity e = players->getEntity(0);
	float playerX = hitboxes->get(e).x + (hitboxes->get(e).w * 0.5f);
	float playerY = hitboxes->get(e).y + (hitboxes->get(e).h * 0.5f);
	scene->setRenderOffset(
		floorf(((game->getScreenWidthPixels() * 0.5f) - playerX) * *pixelScaling),
		floorf(((game->getScreenHeightPixels() * 0.5f) - playerY) * *pixelScaling)
	);
}
