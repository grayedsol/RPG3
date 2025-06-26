/**
 * @file TileMapScript.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapScriptResource
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "FileResource.hpp"
#include "TileMapCommand.hpp"

namespace Tile {
	/**
	 * @brief A set of MapCommands that are executed simultaneously.
	 * 
	 */
	using MapCommandSet = std::vector<MapCommand>;
	
	/**
	 * @brief A series of MapCommandSets that are executed sequentially.
	 * 
	 */
	using MapCommandScript = std::vector<MapCommandSet>;

	/**
	 * @brief 
	 * 
	 */
	struct MapScriptResource : public FileResource {
		std::vector<MapCommandScript> scripts;

		MapScriptResource() = default;
		MapScriptResource(const char* path) : FileResource(path) {}

		~MapScriptResource() = default;

		MapScriptResource(const MapScriptResource&) = delete;
		MapScriptResource& operator=(const MapScriptResource&) = delete;

		friend void swap(MapScriptResource& lhs, MapScriptResource& rhs) {
			using std::swap;
			swap(lhs.scripts, rhs.scripts);
		}

		MapScriptResource(MapScriptResource&& other) { swap(*this, other); }

		bool load(GRY_Game* game) final;
	};
};