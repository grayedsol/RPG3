#pragma once
#include "TileRegisterMapCommands.hpp"

static Tile::MapCommand (*const registerTMC_Funcs[])(Tile::EntityMap&, ECS::entity, float, const GRY_JSON::Value&) = {
	registerTMC_None,
	registerTMC_ActorMovePos,
	registerTMC_ActorSetDirection,
	registerTMC_ActorWait,
	registerTMC_PlayerSpeak,
	registerTMC_PlayerTeleport,
	registerTMC_SwitchMap
};