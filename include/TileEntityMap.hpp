/**
 * @file TileEntityMap.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief TileEntityMap
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Tileset.hpp"
#include "TileComponents.hpp"

/**
 * @brief Represents the entities of a tile map.
 * 
 */
struct TileEntityMap : public FileResource {
	using entity = ECS::entity;
	using EntityLayer = std::vector<entity>;

	std::vector<EntityLayer> entityLayers;

	std::vector<Tileset> tilesets;

	TileEntityMap(const char* path) : FileResource(path) {}

	~TileEntityMap() = default;

	TileEntityMap(const TileEntityMap&) = delete;
	TileEntityMap& operator=(const TileEntityMap&) = delete;

	friend void swap(TileEntityMap& lhs, TileEntityMap& rhs) {
		using std::swap;
		swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
		swap(lhs.entityLayers, rhs.entityLayers);
		swap(lhs.tilesets, rhs.tilesets);
	}

	TileEntityMap(TileEntityMap&& other) noexcept { swap(*this, other); }

	bool load(GRY_Game* game) final override;

	bool load(ComponentSet<Actor>& actors);

	bool load(ComponentSet<ActorTexture>& actorTextures);
};
