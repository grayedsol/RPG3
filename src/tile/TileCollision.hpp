/**
 * @file TileCollision.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::Collision
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Tile.hpp"
#include "FileResource.hpp"
#include "SparseSet.hpp"
#include "SDL3/SDL_rect.h"

namespace Tile {
	/**
	 * @brief Holds collision data for a tileset.
	 * 
	 */
	struct Collision : public FileResource {
		/**
		 * @brief Maximum number of collisions allowed per tileset.
		 * 
		 */
		static const TileId MAX_COLLISIONS = 256;

		/**
		 * @brief Container that maps `TileId`s to collision rectangles.
		 * 
		 */
		SparseSet<SDL_FRect, TileId, MAX_COLLISIONS> collisions;

		/**
		 * @brief Default contructor.
		 * 
		 */
		Collision() = default;

		/**
		 * @brief Constructor.
		 * 
		 * @param path Path to the Collision data JSON file.
		 */
		Collision(const char* path) : FileResource(path) {}

		~Collision() = default;

		Collision(const Collision&) = delete;
		Collision& operator=(const Collision&) = delete;

		/**
		 * @brief Swaps the data of to Collision resources.
		 * 
		 * @param lhs A Collision resource.
		 * @param rhs Another Collision resource.
		 */
		friend void swap(Collision& lhs, Collision& rhs) {
			using std::swap;
			swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
			swap(lhs.collisions, rhs.collisions);
		}

		/**
		 * @brief Move constructor.
		 * 
		 * @param other Collision to take data from.
		 */
		Collision(Collision&& other) { swap(*this, other); }

		/**
		 * Load the collision data from the stored collision file path.
		 * 
		 * @param game Associated game class.
		 * 
		 * @returns `true` if the collision data was already loaded, or there was no data to load.
		 * @returns `false` otherwise.
		 */
		bool load(GRY_Game* game) final override;

		/**
		 * @brief Get the collision rect of a tile.
		 * 
		 * @param tile Tile to get collision rect of.
		 * @return The collision rect if the tile has collision, otherwise nullptr.
		 */
		const SDL_FRect* getCollision(TileId tile);
	};
};
