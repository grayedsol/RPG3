/**
 * @file TileMapCommand.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapCommand
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
		MAP_CMD_ACTOR_MOVE_POS,
		MAP_CMD_ACTOR_SET_DIRECTION,
		MAP_CMD_ACTOR_WAIT,
		MAP_CMD_PLAYER_SPEAK,
		MAP_CMD_PLAYER_TELEPORT
	};

	struct TMC_ActorMovePos {
		MapCommandType type = MAP_CMD_ACTOR_MOVE_POS;
		ECS::entity e;
		Position2 startPos = Position2{ 0, 0 };
		Position2 targetPos;
	};

	struct TMC_ActorSetDirection {
		MapCommandType type = MAP_CMD_ACTOR_SET_DIRECTION;
		ECS::entity e;
		Direction direction;
	};

	struct TMC_ActorWait {
		MapCommandType type = MAP_CMD_ACTOR_WAIT;
		ECS::entity e;
		double time;
	};

	struct TMC_PlayerSpeak {
		MapCommandType type = MAP_CMD_PLAYER_SPEAK;
		ECS::entity e;
		unsigned dialogueId;
	};

	struct TMC_PlayerTeleport {
		MapCommandType type = MAP_CMD_PLAYER_TELEPORT;
		ECS::entity e;
		Position2 position;
	};

	/**
	 * @brief Defines commands that can be used for MapScene scripting.
	 * 
	 */
	union MapCommand {
		MapCommandType type;
		ECS::entity e;
		TMC_ActorMovePos actorMovePos;
		TMC_ActorSetDirection actorSetDirection;
		TMC_ActorWait actorWait;
		TMC_PlayerSpeak playerSpeak;
		TMC_PlayerTeleport playerTeleport;
	};

	struct MapCommandList {
		std::vector<MapCommand> commands;
		uint32_t index = 0;
	};

	using MapCommandTypeList = std::tuple<
		MapCommandType,
		TMC_ActorMovePos,
		TMC_ActorSetDirection,
		TMC_ActorWait,
		TMC_PlayerSpeak,
		TMC_PlayerTeleport
	>;
	
	static const char* MapCommandNames[std::tuple_size<MapCommandTypeList>::value] = {
		"None",
		"ActorMovePos",
		"ActorSetDirection",
		"ActorWait",
		"PlayerSpeak",
		"PlayerTeleport"
	};

	struct MapInteraction {
		MapCommand command;
	};

	struct MapCollisionInteraction {
		MapCommand command;
		enum Mode : uint8_t {
			Continuous, Fleeting, PressurePlate
		} mode;
		bool beingPressed = false;
		bool active = false;
	};
};