#pragma once

namespace Battle {
	class BattleScene;

	class PaletteRenderer {
	private:
		BattleScene* scene;
	public:
		PaletteRenderer(BattleScene* scene);

		void process();
	};
}
