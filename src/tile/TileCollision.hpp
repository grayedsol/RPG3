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
		static const TileId MAX_COLLISIONS = 256;

		SparseSet<SDL_FRect, TileId, MAX_COLLISIONS> collisions;

		bool loaded = false;

		Collision() = default;

		Collision(const char* path) : FileResource(path) {}

		~Collision() = default;

		Collision(const Collision&) = delete;
		Collision& operator=(const Collision&) = delete;

		friend void swap(Collision& lhs, Collision& rhs) {
			using std::swap;
			swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
			swap(lhs.collisions, rhs.collisions);
			swap(lhs.loaded, rhs.loaded);
		}

		Collision(Collision&& other) { swap(*this, other); }

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
