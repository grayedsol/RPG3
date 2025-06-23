/**
 * @file TileMapEntityComponentsList.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Defines components needed for an ECS in a TileMap scene.
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "GRY_ECS.hpp"
#include "TileMapCommand.hpp"

namespace Tile {
	/**
	 * @copybrief GRY_ECS
	 * 
	 */
	using MapECS = GRY_ECS<
		Position2,
		Velocity2,
		Hitbox,
		Actor,
		ActorSprite,
		ActorSpriteAnims,
		Player,
		NPC,
		MapInteraction,
		MapCommand,
		MapCommandList
	>;
};