#pragma once
#include "BattleTypes.hpp"
#include "BattleSpriteResource.hpp"

namespace Battle {
	class BattleScene;

	class MonsterAnimator {
	private:
		Monsters* monsters;

		BattleScene* scene;

		const std::vector<SpriteResource>& monsterSpriteResources;

		const float* pixelScaling;
	public:
		MonsterAnimator(BattleScene* scene, Monsters* monsters, const std::vector<SpriteResource>& monsterSpriteResources);

		void process(double delta);

		void render();
	};
}
