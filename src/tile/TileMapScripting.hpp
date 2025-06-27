/**
 * @file TileMapScripting.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief Tile::MapScripting
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "TileMapCommand.hpp"
#include "TileMapECS.hpp"
#include <vector>

namespace Tile {
	class MapScene;

	class MapScripting {
	private:
		MapScene* scene;

		MapECS* ecs;

		std::vector<MapCommand> currentCommands;

		std::vector<std::vector<MapCommand>> currentScript;

		size_t scriptIndex = 0;

		enum {
			GAMEPLAY,
			CUTSCENE
		} mode = GAMEPLAY;
	public:
		MapScripting(MapScene* scene);

		void process(double delta);

		bool executeCommand(MapCommand& command, double delta);
	private:
		void processEntities(double delta);
		
		void processGameplay(double delta);

		void processCutscene(double delta);

		void removeCommand(size_t index);

		bool processActorMovePos(TMC_ActorMovePos& args);

		bool processActorSetDirection(TMC_ActorSetDirection& args);

		bool processActorWait(TMC_ActorWait& args, double delta);

		bool processPlayerSpeak(TMC_PlayerSpeak& args);

		bool processPlayerTeleport(TMC_PlayerTeleport& args);

		bool processSwitchMap(TMC_SwitchMap& args);

		bool processActivateScript(TMC_ActivateScript& args, double delta);

		bool processMoveCamera(TMC_MoveCamera& args, double delta);

		bool processMoveCameraToPlayer(TMC_MoveCameraToPlayer& args, double delta);

		bool processEnablePlayerControls(TMC_EnablePlayerControls& args);

		bool processDisablePlayerControls(TMC_DisablePlayerControls& args);
	};
};
