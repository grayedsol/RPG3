/**
 * @file TileSpriteAnimator.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief 
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileComponents.hpp"

class TileMapScene;

class TileSpriteAnimator {
private:

	ComponentSet<ActorSprite>* sprites;

	const ComponentSet<Actor>* actors;

	ComponentSet<ActorSpriteAnimations>* actorAnimations;

public:
	TileSpriteAnimator(TileMapScene* scene);

	void process(double delta);
};