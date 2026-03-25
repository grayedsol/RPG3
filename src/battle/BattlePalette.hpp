#pragma once
#include "BattleTypes.hpp"

namespace Battle {
	class BattleScene;

	class BattlePalette {
	private:
		BattleScene* scene;

		PageId& currentPage;

		void goToMainPage();
		void goToItemPage();
		void goToSkillPage();

		void itemPageUp();
		void itemPageDown();
	public:
		BattlePalette(BattleScene* scene, PageId& currentPage);

		void process();
	};
}
