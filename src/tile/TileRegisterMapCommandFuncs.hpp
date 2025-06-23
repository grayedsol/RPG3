#pragma once
#include "TileRegisterMapCommands.hpp"

static Tile::MapCommand (*const registerTMC_Funcs[])(Tile::EntityMap&, ECS::entity, const GRY_JSON::Value&) = {
	registerTMC_None,
	registerTMC_ActorMovePos,
	registerTMC_ActorSetDirection,
	registerTMC_ActorWait,
	registerTMC_PlayerSpeak
};