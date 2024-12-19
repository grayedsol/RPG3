/**
 * @file TileCollision.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TileCollision
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Tile.hpp"
#include "FileResource.hpp"
#include "SparseSet.hpp"

/**
 * @brief Holds collision data for a tileset.
 * 
 */
struct TileCollision : public FileResource {
    using TileId = Tile::TileId;
    
    static const TileId MAX_COLLISIONS = 256;

    SparseSet<SDL_FRect, TileId, MAX_COLLISIONS> collisions;

    bool loaded = false;

	TileCollision() = default;

    TileCollision(const char* path) : FileResource(path) {}

    ~TileCollision() = default;

    TileCollision(const TileCollision&) = delete;
    TileCollision& operator=(const TileCollision&) = delete;

    friend void swap(TileCollision& lhs, TileCollision& rhs) {
        using std::swap;
	    swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
        swap(lhs.collisions, rhs.collisions);
        swap(lhs.loaded, rhs.loaded);
    }

    TileCollision(TileCollision&& other) { swap(*this, other); }

    bool load(GRY_Game* game) final override;

    /**
     * @brief Get the collision rect of a tile.
     * 
     * @param tile Tile to get collision rect of.
     * @return The collision rect if the tile has collision, otherwise nullptr.
     */
    const SDL_FRect* getCollision(TileId tile);
};