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
		MAP_CMD_ACTOR_CHANGE_DIALOGUE,
		MAP_CMD_ACTOR_SPEAK,
		MAP_CMD_ACTOR_WAIT_FOR_SPEAK,
		MAP_CMD_PLAYER_TELEPORT,
		MAP_CMD_SWITCH_MAP,
		MAP_CMD_ACTIVATE_SCRIPT,
		MAP_CMD_MOVE_CAMERA,
		MAP_CMD_MOVE_CAMERA_TO_PLAYER,
		MAP_CMD_ENABLE_PLAYER_CONTROLS,
		MAP_CMD_DISABLE_PLAYER_CONTROLS,
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
		"ActorChangeDialogue",
		"ActorSpeak",
		"ActorWaitForSpeak",
		"PlayerTeleport",
		"SwitchMap",
		"ActivateScript",
		"MoveCamera",
		"MoveCameraToPlayer",
		"EnablePlayerControls",
		"DisablePlayerControls"
	};

	struct TMC_None {
		MapCommandType type = MAP_CMD_NONE;
		ECS::entity e;
	};

	/**
	 * @brief Makes an actor walk to a certain position.
	 * 
	 */
	struct TMC_ActorMovePos {
		MapCommandType type = MAP_CMD_ACTOR_MOVE_POS;
		ECS::entity e;
		Position2 targetPos;
		Velocity2 targetVel = Velocity2{ 0, 0 };
	};

	/**
	 * @brief Sets an actor's direction.
	 * 
	 * @details
	 * Instant.
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
	 * @brief Switches the dialogueId of an actor.
	 * 
	 * @details
	 * Instant.
	 */
	struct TMC_ActorChangeDialogue {
		MapCommandType type = MAP_CMD_ACTOR_CHANGE_DIALOGUE;
		ECS::entity e;
		unsigned dialogueId;
	};

	/**
	 * @brief Initiates a text box dialogue between the player and an entity.
	 * 
	 * @details
	 * Instant.
	 * To make a script wait for the dialogue to be played out,
	 * put an ActorWaitForSpeak command for the same entity directly
	 * after this command.
	 */
	struct TMC_ActorSpeak {
		MapCommandType type = MAP_CMD_ACTOR_SPEAK;
		ECS::entity e;
		unsigned dialogueId;
		Direction direction = Direction::DirectionNone;
	};

	/**
	 * @brief Waits for an actor to be done speaking.
	 * 
	 */
	struct TMC_ActorWaitForSpeak {
		MapCommandType type = MAP_CMD_ACTOR_WAIT_FOR_SPEAK;
		ECS::entity e;
	};

	/**
	 * @brief Teleports the player to a specific position.
	 * 
	 * @details
	 * Instant.
	 */
	struct TMC_PlayerTeleport {
		MapCommandType type = MAP_CMD_PLAYER_TELEPORT;
		ECS::entity e;
		Position2 position;
	};
	
	/**
	 * @brief Switches to a new MapScene.
	 * 
	 * @details
	 * Instant.
	 */
	struct TMC_SwitchMap {
		MapCommandType type = MAP_CMD_SWITCH_MAP;
		ECS::entity e;
		Position2 spawnPosition = Position2{ -1, -1 };
		Direction spawnDirection = Direction::DirectionNone;
		unsigned mapScenePathIndex;
	};

	/**
	 * @brief Activates a script.
	 * 
	 * @details
	 * Instant.
	 */
	struct TMC_ActivateScript {
		MapCommandType type = MAP_CMD_ACTIVATE_SCRIPT;
		ECS::entity e = ECS::NONE;
		size_t scriptIndex;
	};

	struct TMC_MoveCamera {
		MapCommandType type = MAP_CMD_MOVE_CAMERA;
		ECS::entity e = ECS::NONE;
		Position2 position;
		float speed = 16.f;
	};

	struct TMC_MoveCameraToPlayer {
		MapCommandType type = MAP_CMD_MOVE_CAMERA_TO_PLAYER;
		ECS::entity e = ECS::NONE;
		float speed = 16.f;
	};

	/**
	 * @brief Enables player map controls.
	 * 
	 * @details
	 * Instant.
	 */
	struct TMC_EnablePlayerControls {
		MapCommandType type = MAP_CMD_ENABLE_PLAYER_CONTROLS;
		ECS::entity e = ECS::NONE;
	};

	/**
	 * @brief Disables player map controls.
	 * 
	 * @details
	 * Instant.
	 */
	struct TMC_DisablePlayerControls{
		MapCommandType type = MAP_CMD_DISABLE_PLAYER_CONTROLS;
		ECS::entity e = ECS::NONE;
	};

	/**
	 * @brief Defines commands that can be used for MapScene scripting.
	 * 
	 */
	union MapCommand {
		TMC_None data;
		TMC_ActorMovePos actorMovePos;
		TMC_ActorSetDirection actorSetDirection;
		TMC_ActorWait actorWait;
		TMC_ActorChangeDialogue actorChangeDialogue;
		TMC_ActorSpeak actorSpeak;
		TMC_ActorWaitForSpeak actorWaitForSpeak;
		TMC_PlayerTeleport playerTeleport;
		TMC_SwitchMap switchMap;
		TMC_ActivateScript activateScript;
		TMC_MoveCamera moveCamera;
		TMC_MoveCameraToPlayer moveCameraToPlayer;
		TMC_EnablePlayerControls enablePlayerControls;
		TMC_DisablePlayerControls disablePlayerControls;
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
			Continuous,
			Fleeting, /* TODO: Test */
			PressurePlate
		} mode;
		bool beingPressed = false;
		bool active = false;
	};
};
