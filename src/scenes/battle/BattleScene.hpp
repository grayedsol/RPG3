/**
 * @file BattleScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief BattleScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "SoundResource.hpp"
#include "GRY_Texture.hpp"
#include "../../battle/BattleTypes.hpp"
#include "../../battle/BattlePalette.hpp"
#include "../../battle/BattlePaletteRenderer.hpp"
#include "../../battle/BattleTimeFlow.hpp"
#include "../../battle/BattleSpriteResource.hpp"
#include "../../battle/BattleFXAnimator.hpp"
#include "../../battle/BattleMonsterAnimator.hpp"
#include "../../battle/BattleActionExecutor.hpp"
#include "../../battle/BattleHUD.hpp"
#include "../../textbox/Fontset.hpp"

class GRY_PixelGame;

namespace Battle {
	struct BattleSceneInfo {};

	enum BattleSceneTextureIndex {
		TEXTURE_INDEX_BATTLE_HUD_MAIN = 0,
		TEXTURE_INDEX_FIGHTER_TIMER = 1,
		TEXTURE_INDEX_RETICLE = 2,
		TEXTURE_INDEX_PALETTE_BOX = 3,
		TEXTURE_INDEX_INFO_BOX = 4
	};

	class BattleScene : public Scene {
	private:
		BattleSceneInfo sceneInfo;

		std::vector<SpriteResource> fxResources;
		
		std::vector<SpriteResource> monsterSpriteResources;

		std::vector<GRY_Texture> textures;

		Fontset font;

		Palette palette;

		PaletteRenderer paletteRenderer;

		BattleHUD hud;

		TimeFlow timeFlow;

		Actors actors;

		Fighters fighters;

		FXAnimator fxAnimator;

		MonsterAnimator monsterAnimator;

		ActionExecutor actionExecutor;

		void setControls() final;

		void killActor(ActorId actor);
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

		GRY_PixelGame* getPixelGame() const { return (GRY_PixelGame*)game; }

		FighterId getCurrentFighter() { return palette.getCurrentFighter(); }

		PageId getCurrentPage() { return palette.getCurrentPage(); }

		Actors& getActors() { return actors; }

		Fighters& getFighters() { return fighters; }

		const SpriteResource& getFXResource(size_t index) const { return fxResources.at(index); }

		const GRY_Texture& getTexture(BattleSceneTextureIndex index) const { return textures.at(index); }

		const Fontset& getFont() { return font; }

		void setActorFlag(ActorId actor, ActorFlag flag);

		void unsetActorFlag(ActorId actor, ActorFlag flag);
	};
}
