/**
 * @file SpriteAnimator.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::SpriteAnimator
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileComponents.hpp"

namespace Tile {
	class MapScene;

	/**
	 * @brief Animates ActorSprites while they are moving.
	 * 
	 */
	class SpriteAnimator {
	private:
		/**
		 * @brief Sprites of actors.
		 * 
		 */
		ComponentSet<ActorSprite>* sprites;

		/**
		 * @brief Actor data.
		 * 
		 */
		const ComponentSet<Actor>* actors;

		/**
		 * @brief Animations of ActorSprites.
		 * 
		 */
		ComponentSet<ActorSpriteAnims>* actorAnimations;

	public:
		/**
		 * @brief Constructor.
		 * 
		 * @param scene Associated MapScene class.
		 */
		SpriteAnimator(MapScene* scene);

		/**
		 * @brief Animate moving actors.
		 * 
		 * @param delta Delta time for game processing, in seconds.
		 */
		void process(double delta);
	};
};
