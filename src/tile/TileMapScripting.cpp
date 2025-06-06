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
	TMC_MoveActorPos moveGirl;
	moveGirl.targetPos = Position2{ 300, 200 };
	moveGirl.e = 1;
	MapCommand moveGirlU{ .moveActorPos = moveGirl };
	currentCommands.push_back(moveGirlU);
}

void Tile::MapScripting::process(double delta) {
	for (int i = 0; i < currentCommands.size(); i++) {
		MapCommand& command = currentCommands[i];

		switch (command.type) {
			case MAP_CMD_MOVE_ACTOR_POS:
				if (processMoveActorPos(command.moveActorPos)) {
					removeCommand(i);
					i--;
				}
				break;
			default:
				break;
		}
	}
}

void Tile::MapScripting::removeCommand(size_t index) {
	currentCommands[index] = currentCommands.back();
	currentCommands.pop_back();
}

bool Tile::MapScripting::processMoveActorPos(TMC_MoveActorPos& args) {
	static int sign[2] = { -1, 1 };
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
