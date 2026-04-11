#pragma once

namespace Battle {
	class BattleScene;

	class BattleHUD {
	private:
		BattleScene* scene;

		const float* pixelScaling;

		float renderX;
		float renderY;
		float renderW;
		float renderH;
	public:
		BattleHUD(BattleScene* scene);

		void init();

		void render();
	};
}