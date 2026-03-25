/**
 * @file BattleScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief BattleScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "SoundResource.hpp"
#include "../../battle/BattleTypes.hpp"
#include "../../battle/BattlePalette.hpp"
#include <stdint.h>

class GRY_PixelGame;

namespace Battle {
	struct BattleSceneInfo {};

	class BattleScene : public Scene {
	private:
		BattleSceneInfo sceneInfo;

		BattlePalette palette;

		uint8_t currentCharacter = 0;
		PageId currentPage = PageId::MainPage;

		void setControls() final;

	public:
		BattleScene(GRY_PixelGame* pGame, const char* scenePath, BattleSceneInfo sceneInfo);

		/**
		 * @copydoc Scene::init
		 *
		 */
		void init() final;

		/**
		 * @copydoc Scene::process
		 */
		void process() final;

		/**
		 * @copydoc Scene::load
		 */
		bool load() final;

		uint8_t getCurrentCharacter() { return currentCharacter; }

		PageId getCurrentPage() { return currentPage; }
	};
}
