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
			break;
		case CUTSCENE:
			processCutscene(delta);
			break;
		default:
			break;
	}
	processEntities(delta);
}

void Tile::MapScripting::processEntities(double delta) {
	for (auto e : ecs->getComponent<MapCommand>()) {
		MapCommand& command = ecs->getComponent<MapCommand>().get(e);
		if (executeCommand(command, delta)) {
			command = MapCommand{ .type = MAP_CMD_NONE };
		}
	}
}

void Tile::MapScripting::processGameplay(double delta) {
	for (auto e : ecs->getComponent<MapCommandList>()) {
		MapCommandList& commandList = ecs->getComponent<MapCommandList>().get(e);
		GRY_Assert(ecs->getComponent<MapCommand>().contains(e),
		"[Tile::MapScripting] An entity had a MapCommandList component, but no MapCommand component.\n");

		if (ecs->getComponent<MapCommand>().get(e).type == MAP_CMD_NONE) {
			ecs->getComponent<MapCommand>().get(e) = commandList.commands[commandList.index];
			if (++commandList.index >= commandList.commands.size()) {
				commandList.index = 0;
			}
		}
	}
}

void Tile::MapScripting::processCutscene(double delta) {
	for (int i = 0; i < currentCommands.size(); i++) {
		MapCommand& command = currentCommands[i];
		GRY_Assert(ecs->getComponent<MapCommand>().contains(command.e),
		"[Tile::MapScripting] An entity in the script had no MapCommand component.\n");

		if (ecs->getComponent<MapCommand>().get(command.e).type == MAP_CMD_NONE) {
			ecs->getComponent<MapCommand>().get(command.e) = command;
			currentCommands[i] = currentCommands.back();
			currentCommands.pop_back();
			i--;
		}
	}
}

bool Tile::MapScripting::executeCommand(MapCommand& command, double delta) {
	switch (command.type) {
		case MAP_CMD_NONE:
			return false;
		case MAP_CMD_ACTOR_MOVE_POS:
			return processActorMovePos(command.actorMovePos);
		case MAP_CMD_ACTOR_SET_DIRECTION:
			return processActorSetDirection(command.actorSetDirection);
		case MAP_CMD_ACTOR_WAIT:
			return processActorWait(command.actorWait, delta);
		case MAP_CMD_PLAYER_SPEAK:
			return processPlayerSpeak(command.playerSpeak);
		case MAP_CMD_PLAYER_TELEPORT:
			return processPlayerTeleport(command.playerTeleport);
		case MAP_CMD_SWITCH_MAP:
			return processSwitchMap(command.switchMap);
		default:
			GRY_Assert(false, "There was an attempt to execute an unknown MapCommand.\n");
			return false;
	}
}

void Tile::MapScripting::removeCommand(size_t index) {
	currentCommands[index] = currentCommands.back();
	currentCommands.pop_back();
}

bool Tile::MapScripting::processActorMovePos(TMC_ActorMovePos& args) {
	static int sign[2] = { -1, 1 };
	if (args.e == ecs->getComponent<Player>().value[0].speakingTo) { return false; }
	Position2& pos = ecs->getComponent<Position2>().get(args.e);
	if (pos == args.targetPos) { return true; }
	if (args.startPos == Position2{ 0, 0 }) { args.startPos = pos; }

	Velocity2 vel = args.targetPos - pos;
	vel.x = (bool)vel.x * sign[vel.x > 0];
	vel.y = (bool)vel.y * sign[vel.y > 0];

	Velocity2 vel2 = args.targetPos - args.startPos;
	vel2.x = (bool)vel2.x * sign[vel2.x > 0];
	vel2.y = (bool)vel2.y * sign[vel2.y > 0];

	for (int i = 0; i < 2; i++) {
		if (vel[i] * vel2[i] <= 0) {
			vel[i] = 0;
			pos[i] = args.targetPos[i];
		}
	}

	if (vel == Velocity2{ 0, 0 }) {
		ecs->getComponent<Actor>().get(args.e).moving = false;
		return true;
	}

	ecs->getComponent<Actor>().get(args.e).direction = vecToDir(vel);
	ecs->getComponent<Actor>().get(args.e).moving = true;
	return false;
}

bool Tile::MapScripting::processActorSetDirection(TMC_ActorSetDirection& args) {
	if (args.e == ecs->getComponent<Player>().value[0].speakingTo) { return false; }
	if (args.direction != Direction::DirectionNone && args.direction != Direction::DirectionSize) {
		ecs->getComponent<Actor>().get(args.e).direction = args.direction;
	}
	return true;
}

bool Tile::MapScripting::processActorWait(TMC_ActorWait& args, double delta) {
	if (args.e == ecs->getComponent<Player>().value[0].speakingTo) { return false; }
	return (args.time -= delta) <= 0.f;
}

bool Tile::MapScripting::processPlayerSpeak(TMC_PlayerSpeak& args) {
	auto& actors = ecs->getComponent<Actor>();
	auto& players = ecs->getComponent<Player>();
	if (actors.contains(args.e)) {
		actors.get(args.e).direction = invDirs[actors.get(players.getEntity(0)).direction];
		actors.get(args.e).moving = false;
	}
	players.get(players.getEntity(0)).speakingTo = args.e;
	scene->getTileMapSpeak().speak(args.dialogueId);
	return true;
}

bool Tile::MapScripting::processPlayerTeleport(TMC_PlayerTeleport& args) {
	entity player = ecs->getComponent<Player>().getEntity(0);
	ecs->getComponent<Position2>().get(player) = args.position;
	return true;
}

bool Tile::MapScripting::processSwitchMap(TMC_SwitchMap& args) {
	scene->switchMap(args.mapScenePath);
	return true;
}
