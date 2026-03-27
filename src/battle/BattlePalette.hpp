#pragma once
#include "BattleTypes.hpp"

namespace Battle {
	class BattleScene;

	class Palette {
	private:
		BattleScene* scene;

		FighterId currentFighter = FighterId::Fighter0;
		PageId currentPage = PageId::MainPage;

		void beginMove(FighterId fighter, FighterMove move);

		void switchPage(PageId page) { currentPage = page; }

		void itemPageUp();
		void itemPageDown();

		/**
		 * @brief Get the next fighter that is in the idle state that comes after `afterFighter`.
		 * 
		 * @return The next idle fighter, or FighterId::MaxFighters if there are no idle fighters.
		 */
		FighterId getNextIdleFighter(FighterId afterFighter);
	public:
		Palette(BattleScene* scene);

		void process();

		FighterId getCurrentFighter() { return currentFighter; }

		PageId getCurrentPage() { return currentPage; }
	};
}
