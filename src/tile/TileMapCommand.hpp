/**
 * @file TileMapCommand.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief Tile::MapCommand
 * @copyright Copyright (c) 2025
 */
#pragma once
#include <stdint.h>
#include "ECS.hpp"
#include "Components.hpp"

namespace Tile {
	enum MapCommandType : uint32_t {
		MAP_CMD_MOVE_ACTOR_POS
	};

	struct TMC_MoveActorPos {
		MapCommandType type = MAP_CMD_MOVE_ACTOR_POS;
		Position2 startPos = Position2{ 0, 0 };
		Position2 targetPos;
		ECS::entity e;
	};

	/**
	 * @brief Defines commands that can be used for MapScene scripting.
	 * 
	 */
	union MapCommand {
		MapCommandType type;
		TMC_MoveActorPos moveActorPos;
	};
};