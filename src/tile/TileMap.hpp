/**
 * @file TileMap.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::TileMap
 * @copyright Copyright (c) 2024
 */

#pragma once
#include "Tileset.hpp"
#include "TileCollision.hpp"

namespace Tile {
	using TileLayer = std::vector<Tile>;
	using RectangleLayer = std::vector<SDL_FRect>;
	/**
	 * @brief Represents a map in terms of simple tiles.
	 * 
	 */
	struct TileMap : public FileResource {
		/**
		 * @brief Container for layers of tiles.
		 * 
		 */
		std::vector<TileLayer> tileLayers;

		/**
		 * @brief Container for collision rectangles that can span multiple tiles.
		 * 
		 */
		std::vector<RectangleLayer> collisionRects;

		/**
		 * @brief The tileset of the Map.
		 * 
		 */
		Tileset tileset;

		/**
		 * @brief Tile collision data for the Tileset.
		 * 
		 */
		Collision tileCollision;
		
		/**
		 * @brief Width of the map, in tiles.
		 * 
		 */
		uint32_t width;

		/**
		 * @brief Height of the map, in tiles.
		 * 
		 * @details
		 * If there are layers of different heights, the height
		 * is equal to the height of the largest layer.
		 */
		uint32_t height;
		
		/**
		 * @brief Constructor.
		 * 
		 * @copydetails FileResource::FileResource
		 */
		TileMap() = default;

		/**
		 * @brief Constructor.
		 * 
		 * @param path File path to Tiled Map JSON data.
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
			swap(lhs.collisionRects, rhs.collisionRects);
			swap(lhs.tileset, rhs.tileset);
			swap(lhs.tileCollision, rhs.tileCollision);
			swap(lhs.width, rhs.width);
		}

		TileMap(TileMap&& other) noexcept { swap(*this, other); }

		bool load(GRY_Game* game) final override;
	};
};
