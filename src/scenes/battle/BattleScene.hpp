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

		Palette palette;

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

		FighterId getCurrentFighter() { return palette.getCurrentFighter(); }

		PageId getCurrentPage() { return palette.getCurrentPage(); }
	};
}
