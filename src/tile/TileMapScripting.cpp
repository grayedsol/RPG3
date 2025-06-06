/**
 * @file TileMapScripting.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TileMapScripting.hpp"
#include "../scenes/TileMapScene.hpp"

Tile::MapScripting::MapScripting(MapScene *scene) :
	scene(scene),
	ecs(&scene->getECS()) {
}

void Tile::MapScripting::process(double delta) {
	for (int i = 0; i < currentCommands.size(); i++) {
		MapCommand command = currentCommands[i];

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
	for (auto command : currentCommands) {
		switch (command.type) {
			case MAP_CMD_MOVE_ACTOR_POS:
				processMoveActorPos(command.moveActorPos);
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

bool Tile::MapScripting::processMoveActorPos(TMC_MoveActorPos args) {
	static int sign[2] = { -1, 1 };
	Position2 pos = ecs->getComponent<Position2>().get(args.e);
	Velocity2 vel = pos - args.targetPos;
	vel.x = (bool)vel.x * sign[vel.x > 0];
	vel.y = (bool)vel.y * sign[vel.y > 0];
	return true;
}
