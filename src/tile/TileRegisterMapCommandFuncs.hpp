#pragma once
#include "TileRegisterMapCommands.hpp"

static Tile::MapCommand (*const registerTMC_Funcs[])(float, const GRY_JSON::Value&, ECS::entity) = {
	registerTMC_None,
	registerTMC_ActorMovePos,
	registerTMC_ActorSetDirection,
	registerTMC_ActorWait,
	registerTMC_ActorChangeDialogue,
	registerTMC_ActorSpeak,
	registerTMC_ActorWaitForSpeak,
	registerTMC_PlayerTeleport,
	registerTMC_SwitchMap,
	registerTMC_ActivateScript,
	registerTMC_MoveCamera,
	registerTMC_MoveCameraToPlayer,
	registerTMC_EnablePlayerControls,
	registerTMC_DisablePlayerControls
};