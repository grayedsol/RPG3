#pragma once

namespace Battle {
	class BattleScene;

	class PaletteRenderer {
	private:
		BattleScene* scene;

		const float* pixelScaling;
	public:
		PaletteRenderer(BattleScene* scene);

		void process();
	};
}
