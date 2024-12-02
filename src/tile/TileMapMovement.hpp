/**
 * @file TileMapMovement.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief TileMapMovement
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileEntityMap.hpp"
#include "GRY_PixelGame.hpp"

class TileMapScene;

class TileMapMovement {
private:
	TileMapScene* scene;
	ComponentSet<Position2>& positions;
	ComponentSet<Velocity2>& velocities;
	ComponentSet<Actor>& actors;
	ComponentSet<ActorSprite>& sprites;
	const ComponentSet<Player>& players;
public:
	TileMapMovement(TileMapScene* scene);

	void process(double delta);
};