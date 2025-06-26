/**
 * @file TileMapCommand.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapCommand
 * @copyright Copyright (c) 2025
 */
#pragma once
#include <stdint.h>
#include "ECS.hpp"
#include "Components.hpp"
#include "TileComponents.hpp"

namespace Tile {
	/**
	 * @brief Type identifier for the MapCommand union.
	 * 
	 */
	enum MapCommandType : uint32_t {
		MAP_CMD_NONE,
		MAP_CMD_ACTOR_MOVE_POS,
		MAP_CMD_ACTOR_SET_DIRECTION,
		MAP_CMD_ACTOR_WAIT,
		MAP_CMD_PLAYER_SPEAK,
		MAP_CMD_PLAYER_TELEPORT,
		MAP_CMD_SWITCH_MAP,
		MAP_CMD_SIZE
	};

	/**
	 * @brief A list of strings of MapCommands names.
	 * 
	 */
	static const char* MapCommandNames[MapCommandType::MAP_CMD_SIZE] = {
		"None",
		"ActorMovePos",
		"ActorSetDirection",
		"ActorWait",
		"PlayerSpeak",
		"PlayerTeleport",
		"SwitchMap"
	};

	/**
	 * @brief Makes an actor walk to a certain position.
	 * 
	 */
	struct TMC_ActorMovePos {
		MapCommandType type = MAP_CMD_ACTOR_MOVE_POS;
		ECS::entity e;
		Position2 startPos = Position2{ 0, 0 };
		Position2 targetPos;
	};

	/**
	 * @brief Sets an actor's direction.
	 * 
	 */
	struct TMC_ActorSetDirection {
		MapCommandType type = MAP_CMD_ACTOR_SET_DIRECTION;
		ECS::entity e;
		Direction direction;
	};

	/**
	 * @brief Makes an actor wait for a time.
	 * 
	 */
	struct TMC_ActorWait {
		MapCommandType type = MAP_CMD_ACTOR_WAIT;
		ECS::entity e;
		double time;
	};

	/**
	 * @brief Initiates a text box dialogue between the player and an entity.
	 * 
	 */
	struct TMC_PlayerSpeak {
		MapCommandType type = MAP_CMD_PLAYER_SPEAK;
		ECS::entity e;
		unsigned dialogueId;
	};

	/**
	 * @brief Teleports the player to a specific position.
	 * 
	 */
	struct TMC_PlayerTeleport {
		MapCommandType type = MAP_CMD_PLAYER_TELEPORT;
		ECS::entity e;
		Position2 position;
	};
	
	/**
	 * @brief Switches to a new MapScene.
	 * 
	 */
	struct TMC_SwitchMap {
		static const unsigned MAX_PATH_LEN = 127;
		MapCommandType type = MAP_CMD_SWITCH_MAP;
		ECS::entity e;
		Position2 spawnPosition = Position2{ -1, -1 };
		Direction spawnDirection = Direction::DirectionNone;
		char mapScenePath[MAX_PATH_LEN + 1] = { 0 };
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
		TMC_SwitchMap switchMap;
	};

	/**
	 * @brief A list of MapCommands that entities activate sequentially and cyclically.
	 * 
	 */
	struct MapCommandList {
		std::vector<MapCommand> commands;
		uint32_t index = 0;
	};

	/**
	 * @brief A MapCommand that is activated when the player interacts with the entity.
	 * 
	 */
	struct MapInteraction {
		MapCommand command;
	};

	/**
	 * @brief A MapCommand that is activated when the player collides with the entity.
	 * 
	 */
	struct MapCollisionInteraction {
		MapCommand command;
		enum Mode : uint8_t {
			Continuous, Fleeting, PressurePlate
		} mode;
		bool beingPressed = false;
		bool active = false;
	};
};
