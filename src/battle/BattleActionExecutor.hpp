#pragma once

namespace Battle {
	class BattleScene;

	class ActionExecutor {
	private:
		BattleScene* scene;
	public:
		ActionExecutor(BattleScene* scene);

		void process();
	};
}