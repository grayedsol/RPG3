#include "BattlePaletteRenderer.hpp"
#include "../scenes/battle/BattleScene.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL_RectOps.hpp"
#include "SDL3/SDL_render.h"

static const unsigned numPaletteDirections = 4;
static const float renderX[numPaletteDirections] = { 109, 65, 153, 109 };
static const float renderY[numPaletteDirections] = { 185, 212, 212, 239 };

Battle::PaletteRenderer::PaletteRenderer(BattleScene* scene)
	: scene(scene)
	, pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void Battle::PaletteRenderer::process() {
	if (scene->getCurrentFighter() == FighterId::MaxFighters) { return; }

	SDL_Renderer* renderer = scene->getGame()->getVideo().getRenderer();

	const GRY_Texture& boxTexture = scene->getTexture(BattleSceneTextureIndex::TEXTURE_INDEX_PALETTE_BOX);

	const float width = boxTexture.texture->w;
	const float height = boxTexture.texture->h;

	for (int i = 0; i < numPaletteDirections; i++) {
		SDL_FRect dstRect {
			renderX[i], renderY[i], width, height
		};
		dstRect *= *pixelScaling;

		SDL_RenderTexture(renderer, boxTexture.texture, NULL, &dstRect);
	}
}