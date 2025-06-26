#pragma once
#include "TileRegisterMapCommands.hpp"

static Tile::MapCommand (*const registerTMC_Funcs[])(float, const GRY_JSON::Value&, ECS::entity) = {
	registerTMC_None,
	registerTMC_ActorMovePos,
	registerTMC_ActorSetDirection,
	registerTMC_ActorWait,
	registerTMC_PlayerSpeak,
	registerTMC_PlayerTeleport,
	registerTMC_SwitchMap,
	registerTMC_ActivateScript
};