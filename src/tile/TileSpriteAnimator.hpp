/**
 * @file TileSpriteAnimator.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TileSpriteAnimator
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileComponents.hpp"

class TileMapScene;

/**
 * @brief Animates ActorSprites while they are moving.
 * 
 */
class TileSpriteAnimator {
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
	 * @param scene Associated TileMapScene class.
	 */
	TileSpriteAnimator(TileMapScene* scene);

	/**
	 * @brief Animate moving actors.
	 * 
	 * @param delta Delta time for game processing, in seconds.
	 */
	void process(double delta);
};