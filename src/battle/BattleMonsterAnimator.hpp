#pragma once
#include "BattleTypes.hpp"

namespace Battle {
	class BattleScene;

	class MonsterAnimator {
	private:
		BattleScene* scene;
	public:
		MonsterAnimator(BattleScene* scene);

		void process(double delta);
	};
}
