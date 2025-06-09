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

		enum {
			GAMEPLAY,
			CUTSCENE
		} mode = GAMEPLAY;
	public:
		MapScripting(MapScene* scene);

		void process(double delta);
	private:
		void processEntities(double delta);
		
		void processGameplay(double delta);

		void processCutscene(double delta);

		void removeCommand(size_t index);

		bool processMoveActorPos(TMC_MoveActorPos& args);

		bool processSetActorDirection(TMC_SetActorDirection& args);

		bool processWaitActor(TMC_WaitActor& args, double delta);
	};
};