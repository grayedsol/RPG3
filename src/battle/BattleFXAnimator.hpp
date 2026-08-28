/**
 * @file BattleFXAnimator.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::FXAnimator
 * @copyright Copyright (c) 2026
 */
#pragma once
#include "BattleTypes.hpp"
#include "BattleSpriteResource.hpp"

namespace Battle {
	class BattleScene;

	/**
	 * @brief Animates ActorSprites while they are moving.
	 * 
	 */
	class FXAnimator {
	private:
		BattleScene* scene;

		FX* fx;

		const std::vector<SpriteResource>& fxResources; 

		const float* pixelScaling;
	public:
		/**
		 * @brief Constructor.
		 * 
		 * @param scene Associated BattleScene class.
		 */
		FXAnimator(BattleScene* scene, FX* fx, const std::vector<SpriteResource>& fxResources);

		/**
		 * @brief Animate current FX.
		 * 
		 * @param delta Delta time for game processing, in seconds.
		 */
		void process(double delta);

		/**
		 * @brief Render current FX.
		 * 
		 */
		void render();
	};
};
