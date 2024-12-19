/**
 * @file TileMap.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copydoc TileMap
 * @copyright Copyright (c) 2024
 */

#pragma once
#include "Tileset.hpp"
#include "TileCollision.hpp"

/**
 * @brief Represents a map in terms of simple tiles.
 * 
 */
struct TileMap : public FileResource {
	using TileLayer = std::vector<Tile>;

	/**
	 * @brief Container for layers of tiles.
	 * 
	 */
	std::vector<TileLayer> tileLayers;

	/**
	 * @brief Container for tilesets.
	 * 
	 */
	std::vector<Tileset> tilesets;

	/**
	 * @brief Container for tile collision sets.
	 * 
	 */
	std::vector<TileCollision> tileCollisions;
	
	/**
	 * @brief Width of the map, in tiles.
	 * 
	 */
	uint32_t width;
	
	/**
	 * @brief Constructor.
	 * 
	 * @copydetails FileResource::FileResource
	 */
	TileMap() = default;

	/**
	 * @brief Constructor.
	 * 
	 * @param path File path to Tiled tilemap JSON data.
	 */
	TileMap(const char* path) : FileResource(path) {}

	/**
	 * @brief Destructor.
	 * 
	 */
	~TileMap() = default;

	TileMap(const TileMap&) = delete;
	TileMap& operator=(const TileMap&) = delete;

	friend void swap(TileMap& lhs, TileMap& rhs) {
		using std::swap;
        swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
		swap(lhs.tileLayers, rhs.tileLayers);
		swap(lhs.tilesets, rhs.tilesets);
		swap(lhs.tileCollisions, rhs.tileCollisions);
		swap(lhs.width, rhs.width);
	}

	TileMap(TileMap&& other) noexcept { swap(*this, other); }

	bool load(GRY_Game* game) final override;
};