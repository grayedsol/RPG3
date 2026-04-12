#include "BattleHUD.hpp"
#include "../scenes/battle/BattleScene.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL_RectOps.hpp"
#include "SDL3/SDL_render.h"

Battle::BattleHUD::BattleHUD(BattleScene* scene)
	: scene(scene)
	, pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void Battle::BattleHUD::init() {
	renderer = scene->getGame()->getVideo().getRenderer();
	hudTexture = &scene->getTexture(BattleSceneTextureIndex::TEXTURE_INDEX_BATTLE_HUD_MAIN);
	renderX = (scene->getPixelGame()->getScreenWidthPixels() - hudTexture->texture->w) / 2;
	renderY = scene->getPixelGame()->getScreenHeightPixels() - hudTexture->texture->h;
	renderW = (float)hudTexture->texture->w;
	renderH = (float)hudTexture->texture->h;
}

void Battle::BattleHUD::render() {
	SDL_FRect dstRect {
		renderX, renderY, renderW, renderH
	};

	dstRect *= *pixelScaling;

	SDL_RenderTexture(renderer, hudTexture->texture, NULL, &dstRect);
}