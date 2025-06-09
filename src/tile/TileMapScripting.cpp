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
		bool reset = false;
		switch (command.type) {
			case MAP_CMD_NONE:
				break;
			case MAP_CMD_MOVE_ACTOR_POS:
				reset = processMoveActorPos(command.moveActorPos);
				break;
			case MAP_CMD_SET_ACTOR_DIRECTION:
				reset = processSetActorDirection(command.setActorDirection);
				break;
			case MAP_CMD_WAIT_ACTOR:
				reset = processWaitActor(command.waitActor, delta);
				break;
			default:
				GRY_Assert(false, "An unknown MapCommand was assigned to entity %d.\n", e);
				break;
		}
		if (reset) { command = MapCommand{ .type = MAP_CMD_NONE }; }
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

void Tile::MapScripting::removeCommand(size_t index) {
	currentCommands[index] = currentCommands.back();
	currentCommands.pop_back();
}

bool Tile::MapScripting::processMoveActorPos(TMC_MoveActorPos& args) {
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

bool Tile::MapScripting::processSetActorDirection(TMC_SetActorDirection& args) {
	if (args.direction != Direction::DirectionNone && args.direction != Direction::DirectionSize) {
		ecs->getComponent<Actor>().get(args.e).direction = args.direction;
	}
	return true;
}

bool Tile::MapScripting::processWaitActor(TMC_WaitActor& args, double delta) {
	return (args.time -= delta) <= 0.f;
}
