#pragma once
#include "TileEntityMap.hpp"
#include "GRY_JSON.hpp"

static Tile::MapCommand registerTMC_None(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	GRY_Assert(false, "[Tile::EntityMap] \"%s\" is not a valid MapCommand type.\n", Tile::MapCommandNames[0]);
	Tile::MapCommand command = { .type = Tile::MapCommandType::MAP_CMD_NONE };
	return command;
}

static Tile::MapCommand registerTMC_MoveActorPos(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::TMC_MoveActorPos moveActorPos;
	moveActorPos.e = e;
	moveActorPos.targetPos.x = args["x"].GetFloat();
	moveActorPos.targetPos.y = args["y"].GetFloat();
	Tile::MapCommand command = { .moveActorPos = moveActorPos };
	return command;
}

static Tile::MapCommand registerTMC_SetActorDirection(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::TMC_SetActorDirection setActorDirection;
	setActorDirection.e = e;
	setActorDirection.direction = static_cast<Tile::Direction>(args["direction"].GetInt());
	GRY_Assert(static_cast<Tile::Direction>(setActorDirection.direction) > 0 &&
		static_cast<Tile::Direction>(setActorDirection.direction) < 9,
		"[Tile::EntityMap] SetActorDirection direction must be between 1 and 8 inclusive.\n"
	);
	Tile::MapCommand command = { .setActorDirection = setActorDirection };
	return command;
}

static Tile::MapCommand registerTMC_WaitActor(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::TMC_WaitActor waitActor { .e = e, .time = args["time"].GetFloat() };
	GRY_Assert(waitActor.time >= 0.f, "[Tile::EntityMap] WaitActor time cannot be negative.\n");
	Tile::MapCommand command = { .waitActor = waitActor };
	return command;
}
