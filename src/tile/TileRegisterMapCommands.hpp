#pragma once
#include "TileEntityMap.hpp"
#include "GRY_JSON.hpp"

static Tile::MapCommand registerTMC_None(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	return Tile::MapCommand { .data { Tile::MapCommandType::MAP_CMD_NONE } };
}

static Tile::MapCommand registerTMC_ActorMovePos(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_ActorMovePos actorMovePos;
	actorMovePos.e = e != ECS::NONE ? e : args["e"].GetUint();
	actorMovePos.targetPos = Position2 {
		args["position"].GetArray()[0].GetFloat() * normalTileSize,
		args["position"].GetArray()[1].GetFloat() * normalTileSize
	};
	return Tile::MapCommand { .actorMovePos = actorMovePos };
}

static Tile::MapCommand registerTMC_ActorSetDirection(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_ActorSetDirection actorSetDirection;
	actorSetDirection.e = e != ECS::NONE ? e : args["e"].GetUint();
	actorSetDirection.direction = static_cast<Tile::Direction>(args["direction"].GetInt());
	GRY_Assert(static_cast<Tile::Direction>(actorSetDirection.direction) > 0 &&
		static_cast<Tile::Direction>(actorSetDirection.direction) < 9,
		"[Tile::EntityMap] SetActorDirection direction must be between 1 and 8 inclusive.\n"
	);
	return Tile::MapCommand { .actorSetDirection = actorSetDirection };
}

static Tile::MapCommand registerTMC_ActorWait(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_ActorWait actorWait;
	actorWait.e = e != ECS::NONE ? e : args["e"].GetUint();
	actorWait.time = args["time"].GetFloat();
	GRY_Assert(actorWait.time >= 0.f, "[Tile::EntityMap] WaitActor time cannot be negative.\n");
	return Tile::MapCommand { .actorWait = actorWait };
}

static Tile::MapCommand registerTMC_ActorChangeDialogue(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_ActorChangeDialogue actorChangeDialogue;
	actorChangeDialogue.e = e != ECS::NONE ? e : args["e"].GetUint();
	actorChangeDialogue.dialogueId = args["dialogueId"].GetUint();
	return Tile::MapCommand { .actorChangeDialogue = actorChangeDialogue };
}

static Tile::MapCommand registerTMC_ActorSpeak(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_ActorSpeak actorSpeak;
	actorSpeak.e = e != ECS::NONE ? e : args["e"].GetUint();
	actorSpeak.dialogueId = args["dialogueId"].GetUint();
	if (args.HasMember("direction")) {
		actorSpeak.direction = static_cast<Tile::Direction>(args["direction"].GetUint());
		GRY_Assert(static_cast<Tile::Direction>(actorSpeak.direction) > 0 &&
			static_cast<Tile::Direction>(actorSpeak.direction) < 9,
			"[Tile::EntityMap] ActorSpeak direction must be between 1 and 8 inclusive.\n"
		);
	}
	return Tile::MapCommand { .actorSpeak = actorSpeak };
}

static Tile::MapCommand registerTMC_ActorWaitForSpeak(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_ActorWaitForSpeak actorWaitForSpeak;
	actorWaitForSpeak.e = e != ECS::NONE ? e : args["e"].GetUint();
	GRY_Assert(actorWaitForSpeak.e != ECS::NONE,
		"[Tile::EntityMap] ActorWaitForSpeak did not have an entity to wait for.\n"
	);
	return Tile::MapCommand { .actorWaitForSpeak = actorWaitForSpeak };
}

static Tile::MapCommand registerTMC_PlayerTeleport(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_PlayerTeleport playerTeleport;
	playerTeleport.e = e != ECS::NONE ? e : args["e"].GetUint();
	playerTeleport.position = Position2 {
		args["position"].GetArray()[0].GetFloat() * normalTileSize,
		args["position"].GetArray()[1].GetFloat() * normalTileSize
	};
	return Tile::MapCommand { .playerTeleport = playerTeleport };
}

static Tile::MapCommand registerTMC_SwitchMap(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_SwitchMap switchMap;
	switchMap.e = e != ECS::NONE ? e : args["e"].GetUint();
	if (args.HasMember("spawnPosition")) {
		switchMap.spawnPosition = Position2 {
			args["spawnPosition"].GetArray()[0].GetFloat() * normalTileSize,
			args["spawnPosition"].GetArray()[1].GetFloat() * normalTileSize
		};
	}
	if (args.HasMember("spawnDirection")) {
		switchMap.spawnDirection = static_cast<Tile::Direction>(args["spawnDirection"].GetUint());
	}
	switchMap.mapScenePathIndex = args["mapScenePathIndex"].GetUint();
	return Tile::MapCommand { .switchMap = switchMap };
}

static Tile::MapCommand registerTMC_ActivateScript(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_ActivateScript activateScript;
	activateScript.e = ECS::NONE;
	activateScript.scriptIndex = args["scriptIndex"].GetUint();
	return Tile::MapCommand { .activateScript = activateScript };
}

static Tile::MapCommand registerTMC_MoveCamera(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_MoveCamera moveCamera;
	moveCamera.e = ECS::NONE;
	moveCamera.position = Position2 {
		args["position"].GetArray()[0].GetFloat() * normalTileSize,
		args["position"].GetArray()[1].GetFloat() * normalTileSize
	};
	if (args.HasMember("speed")) {
		moveCamera.speed = args["speed"].GetFloat();
	}
	return Tile::MapCommand { .moveCamera = moveCamera };
}

static Tile::MapCommand registerTMC_MoveCameraToPlayer(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e = ECS::NONE) {
	Tile::TMC_MoveCameraToPlayer moveCameraToPlayer;
	moveCameraToPlayer.e = ECS::NONE;
	if (args.HasMember("speed")) {
		moveCameraToPlayer.speed = args["speed"].GetFloat();
	}
	return Tile::MapCommand { .moveCameraToPlayer = moveCameraToPlayer };
}

static Tile::MapCommand registerTMC_EnablePlayerControls(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e =  ECS::NONE) {
	Tile::TMC_EnablePlayerControls enablePlayerControls;
	return Tile::MapCommand { .enablePlayerControls = enablePlayerControls };
}

static Tile::MapCommand registerTMC_DisablePlayerControls(float normalTileSize, const GRY_JSON::Value& args, ECS::entity e =  ECS::NONE) {
	Tile::TMC_DisablePlayerControls disablePlayerControls;
	return Tile::MapCommand { .disablePlayerControls = disablePlayerControls };
}
