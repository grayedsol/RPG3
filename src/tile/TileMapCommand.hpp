/**
 * @file TileMapCommand.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief Tile::MapCommand
 * @copyright Copyright (c) 2025
 */
#pragma once
#include <stdint.h>
#include <tuple>
#include "ECS.hpp"
#include "Components.hpp"
#include "TileComponents.hpp"

namespace Tile {
	enum MapCommandType : uint32_t {
		MAP_CMD_NONE,
		MAP_CMD_MOVE_ACTOR_POS,
		MAP_CMD_SET_ACTOR_DIRECTION,
		MAP_CMD_WAIT_ACTOR
	};

	struct TMC_MoveActorPos {
		MapCommandType type = MAP_CMD_MOVE_ACTOR_POS;
		ECS::entity e;
		Position2 startPos = Position2{ 0, 0 };
		Position2 targetPos;
	};

	struct TMC_SetActorDirection {
		MapCommandType type = MAP_CMD_SET_ACTOR_DIRECTION;
		ECS::entity e;
		Direction direction;
	};

	struct TMC_WaitActor {
		MapCommandType type = MAP_CMD_WAIT_ACTOR;
		ECS::entity e;
		double time;
	};

	/**
	 * @brief Defines commands that can be used for MapScene scripting.
	 * 
	 */
	union MapCommand {
		MapCommandType type;
		ECS::entity e;
		TMC_MoveActorPos moveActorPos;
		TMC_SetActorDirection setActorDirection;
		TMC_WaitActor waitActor;
	};

	struct MapCommandList {
		std::vector<MapCommand> commands;
		uint32_t index = 0;
	};

	using MapCommandTypeList = std::tuple<
		MapCommandType,
		TMC_MoveActorPos,
		TMC_SetActorDirection,
		TMC_WaitActor
	>;
	
	static const char* MapCommandNames[std::tuple_size<MapCommandTypeList>::value] = {
		"None",
		"MoveActorPos",
		"SetActorDirection",
		"WaitActor"
	};
};