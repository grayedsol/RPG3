/**
 * @file TileEntityMap.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::EntityMap
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Tileset.hpp"
#include "TileMapECS.hpp"

namespace Tile {
	using EntityLayer = std::vector<entity>;
	/**
	 * @brief Represents the entities of a tile map.
	 * 
	 */
	struct EntityMap : public FileResource {
		std::vector<EntityLayer> entityLayers;

		std::vector<Tileset> tilesets;

		MapECS* ecs;

		EntityMap(MapECS& ecs) : ecs(&ecs) {}

		EntityMap(const char* path, MapECS& ecs) : FileResource(path), ecs(&ecs) {}

		~EntityMap() = default;

		EntityMap(const EntityMap&) = delete;
		EntityMap& operator=(const EntityMap&) = delete;

		friend void swap(EntityMap& lhs, EntityMap& rhs) {
			using std::swap;
			swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
			swap(lhs.ecs, rhs.ecs);
			swap(lhs.entityLayers, rhs.entityLayers);
			swap(lhs.tilesets, rhs.tilesets);
		}

		EntityMap(EntityMap&& other) noexcept { swap(*this, other); }

		bool load(GRY_Game* game) final override;

		static void sortLayer(EntityMap* entityMap, unsigned layer);
		static void updateLayers(EntityMap* entityMap);
	};
};
