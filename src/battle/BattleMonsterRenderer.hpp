#pragma once
#include "BattleTypes.hpp"

namespace Battle {
	class BattleScene;

	class MonsterRenderer {
	private:
		BattleScene* scene;
	public:
		MonsterRenderer(BattleScene* scene);

		void process(double delta);
	};
}
