#pragma once
#include "BattleTypes.hpp"

namespace Battle {
	class BattleScene;

	class Palette {
	private:
		BattleScene* scene;

		FighterId currentFighter = FighterId::Fighter0;
		PageId currentPage = PageId::MainPage;

		void goToMainPage();
		void goToItemPage();
		void goToSkillPage();

		void itemPageUp();
		void itemPageDown();

		void switchFighters();
	public:
		Palette(BattleScene* scene);

		void process();

		FighterId getCurrentFighter() { return currentFighter; }

		PageId getCurrentPage() { return currentPage; }
	};
}
