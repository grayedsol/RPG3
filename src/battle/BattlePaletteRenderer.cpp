#include "BattlePaletteRenderer.hpp"
#include "../scenes/battle/BattleScene.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL_RectOps.hpp"
#include "SDL3/SDL_render.h"

Battle::PaletteRenderer::PaletteRenderer(BattleScene* scene)
	: scene(scene)
	, pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void Battle::PaletteRenderer::init() {
	renderer = scene->getGame()->getVideo().getRenderer();
	paletteBoxTexture = &scene->getTexture(BattleSceneTextureIndex::TEXTURE_INDEX_PALETTE_BOX);
	renderW = paletteBoxTexture->texture->w;
	renderH = paletteBoxTexture->texture->h;
}

void Battle::PaletteRenderer::process() {
	if (scene->getCurrentFighter() == FighterId::MaxFighters) { return; }

	for (int i = 0; i < numDirections; i++) {
		SDL_FRect dstRect {
			renderX[i], renderY[i], renderW, renderH
		};
		dstRect *= *pixelScaling;

		SDL_RenderTexture(renderer, paletteBoxTexture->texture, NULL, &dstRect);
	}
}

void Battle::PaletteRenderer::setRenderPos(unsigned index, float x, float y) {
	renderX[index] = x;
	renderY[index] = y;
}
