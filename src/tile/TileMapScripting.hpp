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
	public:
		MapScripting(MapScene* scene);

		void process(double delta);
	private:
		void removeCommand(size_t index);

		bool processMoveActorPos(TMC_MoveActorPos args);
	};
};