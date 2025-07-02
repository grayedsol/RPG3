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

	/**
	 * @brief Runs MapCommands during gameplay and cutscenes.
	 * 
	 */
	class MapScripting {
	private:
		/**
		 * @brief Associated MapScene.
		 * 
		 */
		MapScene* scene;

		/**
		 * @brief The ECS database of the MapScene.
		 * 
		 */
		MapECS* ecs;

		/**
		 * @brief A series of sets of MapCommands representing the running cutscene script.
		 * 
		 */
		std::vector<std::vector<MapCommand>> currentScript;

		/**
		 * @brief The index of the current set of MapCommands in the script.
		 * 
		 */
		size_t scriptIndex = 0;

		/**
		 * @brief Current scripting mode.
		 * 
		 */
		enum {
			GAMEPLAY,
			CUTSCENE
		} mode = GAMEPLAY;
	public:
		/**
		 * @brief Constructor.
		 * 
		 * @param scene Associated MapScene.
		 */
		MapScripting(MapScene* scene);

		/**
		 * @brief Process all MapCommands and either the Gameplay or Cutscene mode.
		 * 
		 * @param delta Delta time for game processing, in seconds.
		 */
		void process(double delta);

		/**
		 * @brief 
		 * 
		 * @param command MapCommand to execute.
		 * @param delta Delta time for game processing, in seconds.
		 * @return `true` if the command has executed completely.
		 * @return `false` if the command still needs to be run more times before it is finished.
		 */
		bool executeCommand(MapCommand& command, double delta);

		bool inCutscene() const { return mode == CUTSCENE; }
	private:
		void processEntities(double delta);
		
		void processGameplay(double delta);

		void processCutscene(double delta);

		bool processActorMovePos(TMC_ActorMovePos& args);

		bool processActorSetDirection(TMC_ActorSetDirection& args);

		bool processActorWait(TMC_ActorWait& args, double delta);

		bool processActorChangeDialogue(TMC_ActorChangeDialogue& args);

		bool processActorSpeak(TMC_ActorSpeak& args);

		bool processActorWaitForSpeak(TMC_ActorWaitForSpeak& args);

		bool processPlayerTeleport(TMC_PlayerTeleport& args);

		bool processSwitchMap(TMC_SwitchMap& args);

		bool processActivateScript(TMC_ActivateScript& args, double delta);

		bool processMoveCamera(TMC_MoveCamera& args, double delta);

		bool processMoveCameraToPlayer(TMC_MoveCameraToPlayer& args, double delta);

		bool processEnablePlayerControls(TMC_EnablePlayerControls& args);

		bool processDisablePlayerControls(TMC_DisablePlayerControls& args);
	};
};
