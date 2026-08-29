#include "BattleMonsterAnimationsResource.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"
#include "SDL3/SDL_render.h"

bool Battle::MonsterAnimationsResource::load(GRY_Game* game) {
	if (!durations.empty()) { return true; }

	/* MonsterSpriteIds are 1-indexed, so insert empty animation data at the 0-index. */
	durations.push_back(MonsterAnimationsDurations());
	indices.push_back(MonsterAnimationsIndices());

	/* Open the animations file */
	GRY_JSON::Document animationsDoc;
	GRY_JSON::loadDoc(animationsDoc, path);

	for (auto& monster : animationsDoc["monsters"].GetArray()) {
		MonsterAnimationsDurations animationsDurations;
		MonsterAnimationsIndices animationsIndices;
		for (auto& animation : monster["animations"].GetArray()) {
			AnimationDurations animationDurations;
			AnimationIndices animationIndices;
			for (auto& duration : animation["durations"].GetArray()) {
				animationDurations.push_back(duration.GetDouble());
			}
			for (auto& index : animation["indices"].GetArray()) {
				animationIndices.push_back(index.GetUint());
			}
			animationsDurations.push_back(animationDurations);
			animationsIndices.push_back(animationIndices);
		}
		durations.push_back(animationsDurations);
		indices.push_back(animationsIndices);
	}

	return animationsDoc["monsters"].GetArray().Size() == 0;
}