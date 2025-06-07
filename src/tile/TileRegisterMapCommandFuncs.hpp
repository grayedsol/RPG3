#pragma once
#include "TileRegisterMapCommands.hpp"

static Tile::MapCommand (*const registerTMC_Funcs[])(Tile::EntityMap&, ECS::entity, const GRY_JSON::Value&) = {
	registerTMC_None,
	registerTMC_MoveActorPos,
	registerTMC_SetActorDirection,
	registerTMC_WaitActor
};