/**
 * @file TileMapScripting.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TileMapScripting.hpp"
#include "../scenes/TileMapScene.hpp"

static Tile::Direction vecDirs[Tile::Direction::DirectionSize] {
	Tile::Direction::LeftUp,
	Tile::Direction::Left,
	Tile::Direction::LeftDown,
	Tile::Direction::Up,
	Tile::Direction::DirectionNone,
	Tile::Direction::Down,
	Tile::Direction::RightUp,
	Tile::Direction::Right,
	Tile::Direction::RightDown
};

static Tile::Direction invDirs[Tile::Direction::DirectionSize] {
	Tile::Direction::DirectionNone,
	Tile::Direction::Up,
	Tile::Direction::Down,
	Tile::Direction::Right,
	Tile::Direction::RightUp,
	Tile::Direction::RightDown,
	Tile::Direction::Left,
	Tile::Direction::LeftUp,
	Tile::Direction::LeftDown
};

static Tile::Direction vecToDir(Velocity2 vec) {
	return vecDirs[(int)((vec[0]+1)*3+vec[1]+1)];
}

Tile::MapScripting::MapScripting(MapScene *scene) :
	scene(scene),
	ecs(&scene->getECS()) {
}

void Tile::MapScripting::process(double delta) {
	switch (mode) {
		case GAMEPLAY:
			processGameplay(delta);
			processEntities(delta);
			break;
		case CUTSCENE:
			processCutscene(delta);
			break;
		default:
			break;
	}
}

void Tile::MapScripting::processEntities(double delta) {
	for (auto e : ecs->getComponent<MapCommand>()) {
		MapCommand& command = ecs->getComponent<MapCommand>().get(e);
		if (executeCommand(command, delta)) {
			command = MapCommand{ .data { MAP_CMD_NONE } };
		}
	}
}

void Tile::MapScripting::processGameplay(double delta) {
	for (auto e : ecs->getComponent<MapCommandList>()) {
		MapCommandList& commandList = ecs->getComponent<MapCommandList>().get(e);
		GRY_Assert(ecs->getComponent<MapCommand>().contains(e),
		"[Tile::MapScripting] An entity had a MapCommandList component, but no MapCommand component.\n");

		if (ecs->getComponent<MapCommand>().get(e).data.type == MAP_CMD_NONE) {
			ecs->getComponent<MapCommand>().get(e) = commandList.commands[commandList.index];
			if (++commandList.index >= commandList.commands.size()) {
				commandList.index = 0;
			}
		}
	}
}

void Tile::MapScripting::processCutscene(double delta) {
	bool currentScriptComplete = true;
	for (auto& command : currentScript.at(scriptIndex)) {
		if (command.data.type == MAP_CMD_NONE) { continue; }

		currentScriptComplete = false;
		if (executeCommand(command, delta)) {
			command = MapCommand { .data { MAP_CMD_NONE } };
		}
		continue;
	}
	if (!currentScriptComplete) { return; }

	if (++scriptIndex >= currentScript.size()) {
		mode = GAMEPLAY;
		scriptIndex = 0;
	}
}

bool Tile::MapScripting::executeCommand(MapCommand& command, double delta) {
	switch (command.data.type) {
		case MAP_CMD_NONE:
			return false;
		case MAP_CMD_ACTOR_MOVE_POS:
			return processActorMovePos(command.actorMovePos);
		case MAP_CMD_ACTOR_SET_DIRECTION:
			return processActorSetDirection(command.actorSetDirection);
		case MAP_CMD_ACTOR_WAIT:
			return processActorWait(command.actorWait, delta);
		case MAP_CMD_ACTOR_CHANGE_DIALOGUE:
			return processActorChangeDialogue(command.actorChangeDialogue);
		case MAP_CMD_ACTOR_SPEAK:
			return processActorSpeak(command.actorSpeak);
		case MAP_CMD_ACTOR_WAIT_FOR_SPEAK:
			return processActorWaitForSpeak(command.actorWaitForSpeak);
		case MAP_CMD_PLAYER_TELEPORT:
			return processPlayerTeleport(command.playerTeleport);
		case MAP_CMD_SWITCH_MAP:
			return processSwitchMap(command.switchMap);
		case MAP_CMD_ACTIVATE_SCRIPT:
			return processActivateScript(command.activateScript, delta);
		case MAP_CMD_MOVE_CAMERA:
			return processMoveCamera(command.moveCamera, delta);
		case MAP_CMD_MOVE_CAMERA_TO_PLAYER:
			return processMoveCameraToPlayer(command.moveCameraToPlayer, delta);
		case MAP_CMD_ENABLE_PLAYER_CONTROLS:
			return processEnablePlayerControls(command.enablePlayerControls);
		case MAP_CMD_DISABLE_PLAYER_CONTROLS:
			return processDisablePlayerControls(command.disablePlayerControls);
		default:
			GRY_Assert(false, "There was an attempt to execute an unknown MapCommand.\n");
			return false;
	}
}

bool Tile::MapScripting::processActorMovePos(TMC_ActorMovePos& args) {
	static const int sign[2] = { -1, 1 };

	/* If being spoken to, don't move */
	if (args.e == ecs->getComponent<Player>().value[0].speakingTo) { return false; }

	Position2& pos = ecs->getComponent<Position2>().get(args.e);
	/* If the target position has been reached, return true */
	if (pos == args.targetPos) { return true; }
	/* If the target velocity is at the default/zero value, calculate and set it */
	if(args.targetVel == Velocity2{ 0, 0 }) {
		Velocity2 targetVel = args.targetPos - pos;
		targetVel.x = (bool)targetVel.x * sign[targetVel.x > 0];
		targetVel.y = (bool)targetVel.y * sign[targetVel.y > 0];
		args.targetVel = targetVel;
	}

	/* Construct movement vector */
	Velocity2 vel = args.targetPos - pos;
	vel.x = (bool)vel.x * sign[vel.x > 0];
	vel.y = (bool)vel.y * sign[vel.y > 0];

	/* Check if actor has moved past the target by comparing the signs of movement and target vectors */
	for (int i = 0; i < 2; i++) {
		if (vel[i] * args.targetVel[i] <= 0) {
			/* If there is a mismatch, set movement to 0 and position to the target position */
			vel[i] = 0;
			pos[i] = args.targetPos[i];
		}
	}

	/* Set direction for the movement system to use */
	Direction direction = vecToDir(vel);
	ecs->getComponent<Actor>().get(args.e).movingDirection = direction;
	if (direction) { ecs->getComponent<Actor>().get(args.e).direction = direction; }

	return false;
}

bool Tile::MapScripting::processActorSetDirection(TMC_ActorSetDirection& args) {
	if (ecs->getComponent<Player>().value[0].speakingTo == args.e) { return false; }
	ecs->getComponent<Actor>().get(args.e).direction = args.direction;
	return true;
}

bool Tile::MapScripting::processActorWait(TMC_ActorWait& args, double delta) {
	return (args.time -= delta) <= 0.f;
}

bool Tile::MapScripting::processActorChangeDialogue(TMC_ActorChangeDialogue &args) {
	auto& cmd = ecs->getComponent<MapInteraction>().get(args.e).command.actorSpeak;
	GRY_Assert(cmd.type == MAP_CMD_ACTOR_SPEAK,
		"[Tile::MapScripting] Tried to change the dialogue of an NPC without a PlayerSpeak MapInteraction.\n"
	);
	cmd.dialogueId = args.dialogueId;
	return true;
}

bool Tile::MapScripting::processActorSpeak(TMC_ActorSpeak& args) {
	auto& actors = ecs->getComponent<Actor>();
	auto& players = ecs->getComponent<Player>();
	if (actors.contains(args.e)) {
		/* If the direction was specified use it, otherwise use the direction opposite the player's */
		actors.get(args.e).direction = args.direction != Direction::DirectionNone ? 
			args.direction :
			invDirs[actors.get(players.getEntity(0)).direction];
	}
	players.get(players.getEntity(0)).speakingTo = args.e;
	scene->getTileMapSpeak().speak(args.dialogueId);
	return true;
}

bool Tile::MapScripting::processActorWaitForSpeak(TMC_ActorWaitForSpeak &args) {
	return ecs->getComponent<Player>().value[0].speakingTo != args.e;
}

bool Tile::MapScripting::processPlayerTeleport(TMC_PlayerTeleport& args) {
	entity player = ecs->getComponent<Player>().getEntity(0);
	ecs->getComponent<Position2>().get(player) = args.position;
	return true;
}

bool Tile::MapScripting::processSwitchMap(TMC_SwitchMap& args) {
	MapSceneInfo mapSceneInfo;
	mapSceneInfo.spawnPosition = args.spawnPosition;
	mapSceneInfo.spawnDirection = args.spawnDirection;
	GRY_Assert(args.mapScenePathIndex < scene->getTileEntityMap().paths.size(),
		"[Tile::MapScripting] SwitchMap command's mapScenePathIndex (%d) was out of bounds.",
		args.mapScenePathIndex
	);
	scene->switchMap(scene->getTileEntityMap().paths.at(args.mapScenePathIndex), mapSceneInfo);
	return true;
}

bool Tile::MapScripting::processActivateScript(TMC_ActivateScript &args, double delta) {
	currentScript = scene->getScriptResource().scripts.at(args.scriptIndex);
	GRY_Assert(mode != CUTSCENE,
		"[Tile::MapScripting] Tried to activate a cutscene while one was already playing.\n"
	);
	mode = CUTSCENE;
	for (auto& cmd : currentScript.front()) {
		if (cmd.data.e == ECS::NONE) { continue; }
		else { ecs->getComponent<MapCommand>().get(cmd.data.e) = MapCommand { .data = { MAP_CMD_NONE } }; }
	}
	return true;
}

bool Tile::MapScripting::processMoveCamera(TMC_MoveCamera &args, double delta) {
	scene->getMapCamera().unlockCamera();
	return scene->getMapCamera().moveCamera(args.position, args.speed, delta);
}

bool Tile::MapScripting::processMoveCameraToPlayer(TMC_MoveCameraToPlayer &args, double delta) {
	if (scene->getMapCamera().moveCameraToPlayer(args.speed, delta)) {
		scene->getMapCamera().lockCamera();
		return true;
	}
	return false;
}

bool Tile::MapScripting::processEnablePlayerControls(TMC_EnablePlayerControls &args) {
	scene->activateControls();
	return true;
}

bool Tile::MapScripting::processDisablePlayerControls(TMC_DisablePlayerControls &args) {
	scene->deactivateControls();
	return true;
}
