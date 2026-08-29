/**
 * @file BattleMonsterAnimator.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2026
 */
#include "BattleMonsterAnimator.hpp"
#include "GRY_PixelGame.hpp"
#include "../scenes/battle/BattleScene.hpp"
#include "SDL3/SDL_render.h"
#include "SDL_RectOps.hpp"

Battle::MonsterAnimator::MonsterAnimator(BattleScene* scene, Monsters* monsters, const std::vector<SpriteResource>& monsterSpriteResources)
	: scene(scene)
	, monsters(monsters)
	, monsterSpriteResources(monsterSpriteResources)
	, pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void Battle::MonsterAnimator::process(double delta) {
	for (uint8_t m = Monster0; m < MaxMonsters; m++) {
		if (monsters->animations[m].durations.empty()) { continue; }

		MonsterSpriteAnimation& animation = monsters->animations[m];
		animation.timer -= delta;
		if (animation.timer <= -delta) {
			if (++animation.currentFrame >= animation.durations.size()) {
				animation = MonsterSpriteAnimation();
				monsters->textureIndex[m] = 0;
			}
			else {
				monsters->textureIndex[m] = animation.indices[animation.currentFrame];
				animation.timer = animation.durations[animation.currentFrame];
			}
		}
	}
}

void Battle::MonsterAnimator::render() {
	for (uint8_t m = Monster0; m < MaxMonsters; m++) {
		if (monsters->globalId[m] == 0 || monsters->textureIndex[m] < 0) { continue; }

		SDL_Renderer* renderer = scene->getGame()->getVideo().getRenderer();
		const SpriteResource& resource = monsterSpriteResources.at(monsters->globalId[m]);
		const SDL_FRect* srcRect = &resource.sourceRects.at(monsters->textureIndex[m]);
		SDL_FRect dstRect {
			0, 0,
			srcRect->w, srcRect->h
		};
		dstRect *= *pixelScaling;
		SDL_RenderTexture(renderer, resource.texture, srcRect, &dstRect);
	}
}