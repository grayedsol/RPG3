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
#include "../../battle/BattlePaletteRenderer.hpp"
#include "../../battle/BattleTimeFlow.hpp"

class GRY_PixelGame;

namespace Battle {
	struct BattleSceneInfo {};

	class BattleScene : public Scene {
	private:
		BattleSceneInfo sceneInfo;

		Palette palette;

		PaletteRenderer paletteRenderer;

		TimeFlow timeFlow;

		void setControls() final;

		Actors actors;

		Fighters fighters;

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

		Actors& getActors() { return actors; }

		Fighters& getFighters() { return fighters; }

		void setActorFlag(ActorId actor, ActorFlag flag);

		void unsetActorFlag(ActorId actor, ActorFlag flag);
	};
}
