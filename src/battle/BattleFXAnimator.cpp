/**
 * @file TileSpriteAnimator.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2026
 */
#include "BattleFXAnimator.hpp"
#include "../scenes/battle/BattleScene.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL3/SDL_render.h"
#include "SDL_RectOps.hpp"

Battle::FXAnimator::FXAnimator(BattleScene* scene, FX* fx, const std::vector<FXResource>& fxResources)
	: scene(scene)
	, fx(fx)
	, fxResources(fxResources)
	, pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void Battle::FXAnimator::process(double delta) {
	for (ActorId a = 0; a < MAX_ACTORS; a++) {
		if (!fx[a].duration) { continue; }

		fx[a].timer -= delta;
		if (fx[a].timer < 0) {
			fx[a].timer = fx[a].duration;
			if (++fx[a].index >= fxResources.at(fx[a].fx).sourceRects.size()) {
				fx[a] = FX();
			}
		}
	}
}

void Battle::FXAnimator::render() {
	for (ActorId a = 0; a < MAX_ACTORS; a++) {
		if (!fx[a].duration) { continue; }
		
		SDL_Renderer* renderer = scene->getGame()->getVideo().getRenderer();
		const FXResource& resource = fxResources.at(fx[a].fx);
		const SDL_FRect* srcRect = &resource.sourceRects.at(fx[a].index);
		SDL_FRect dstRect {
			0, 0,
			srcRect->w, srcRect->h
		};
		dstRect *= *pixelScaling;

		SDL_RenderTexture(renderer, resource.texture, srcRect, &dstRect);
	}
}
