#pragma once
#include "TileEntityMap.hpp"
#include "GRY_JSON.hpp"

static Tile::MapCommand registerTMC_None(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::MapCommand command = { .type = Tile::MapCommandType::MAP_CMD_NONE };
	return command;
}

static Tile::MapCommand registerTMC_ActorMovePos(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::TMC_ActorMovePos actorMovePos;
	actorMovePos.e = e;
	actorMovePos.targetPos.x = args["x"].GetFloat();
	actorMovePos.targetPos.y = args["y"].GetFloat();
	Tile::MapCommand command = { .actorMovePos = actorMovePos };
	return command;
}

static Tile::MapCommand registerTMC_ActorSetDirection(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::TMC_ActorSetDirection actorSetDirection;
	actorSetDirection.e = e;
	actorSetDirection.direction = static_cast<Tile::Direction>(args["direction"].GetInt());
	GRY_Assert(static_cast<Tile::Direction>(actorSetDirection.direction) > 0 &&
		static_cast<Tile::Direction>(actorSetDirection.direction) < 9,
		"[Tile::EntityMap] SetActorDirection direction must be between 1 and 8 inclusive.\n"
	);
	Tile::MapCommand command = { .actorSetDirection = actorSetDirection };
	return command;
}

static Tile::MapCommand registerTMC_ActorWait(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::TMC_ActorWait actorWait { .e = e, .time = args["time"].GetFloat() };
	GRY_Assert(actorWait.time >= 0.f, "[Tile::EntityMap] WaitActor time cannot be negative.\n");
	Tile::MapCommand command = { .actorWait = actorWait };
	return command;
}

static Tile::MapCommand registerTMC_PlayerSpeak(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::TMC_PlayerSpeak playerSpeak { .e = e, .dialogueId = args["dialogueId"].GetUint() };
	Tile::MapCommand command = { .playerSpeak = playerSpeak };
	return command;
}

static Tile::MapCommand registerTMC_PlayerTeleport(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::TMC_PlayerTeleport playerTeleport { .e = e };
	playerTeleport.position = Position2 {
		args["position"].GetArray()[0].GetFloat(),
		args["position"].GetArray()[1].GetFloat()
	};
	return Tile::MapCommand { .playerTeleport = playerTeleport };
}

static Tile::MapCommand registerTMC_SwitchMap(Tile::EntityMap& eMap, ECS::entity e, const GRY_JSON::Value& args) {
	Tile::TMC_SwitchMap switchMap { .e = e };
	GRY_Assert(args["mapScenePath"].GetStringLength() <= Tile::TMC_SwitchMap::MAX_PATH_LEN,
		"[Tile::EntityMap] SwitchMap mapScenePath was too long (max %d)", Tile::TMC_SwitchMap::MAX_PATH_LEN
	);
	strcpy(switchMap.mapScenePath, args["mapScenePath"].GetString());
	return Tile::MapCommand { .switchMap = switchMap };
}