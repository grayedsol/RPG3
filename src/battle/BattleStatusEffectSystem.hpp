#pragma once

namespace Battle {
	class BattleScene;

	class StatusEffectSystem {
	private:
		BattleScene* scene;
	public:
		StatusEffectSystem(BattleScene* scene);
	};
}