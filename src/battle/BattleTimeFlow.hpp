#pragma once
#include "BattleTypes.hpp"

namespace Battle {
	class BattleScene;

	class TimeFlow {
	private:
		BattleScene* scene;
	public:
		TimeFlow(BattleScene* scene);

		void process(double delta);
	};
}
