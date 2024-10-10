/**
 * @file TileMapEntityComponentsList.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Defines components needed for an ECS in a TileMap scene.
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "GRY_ECS.hpp"
#include "Components.hpp"
#include "TileComponents.hpp"

/**
 * @copybrief GRY_ECS
 * 
 */
using TileMapECS = GRY_ECS<
	Position2,
	Velocity2,
	Actor,
	ActorTexture,
	Player
>;