/**
 * @file Tile.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Defines Tile and TileAnimation types
 * @copyright Copyright (c) 2024
 */
#include "SDL3/SDL.h"
#include "ECS.hpp"
#include <vector>

/**
 * @brief A lightweight game entity.
 * 
 */
struct Tile {
    using TileId = uint16_t;
    using entity = ECS::entity;

    /**
     * @brief Identifier to lookup in a tileset.
     * 
     */
    TileId id;

    /**
     * @brief Index of the tileset to use.
     * 
     */
    uint8_t tileset;

    /**
     * @brief Associated entity for more complex features.
     * 
     */
    entity custom;
};

/**
 * @brief Specifications for animating a tile.
 * 
 */
struct TileAnimation {
	using TileId = Tile::TileId;

	struct Frame {
        /**
         * @brief Length of the frame in seconds.
         * 
         */
		const double duration;

        /**
         * @brief Index of source rect to use.
         * 
         */
		const TileId index;
	};
    
    /**
     * @brief Container of animation frames.
     * 
     */
    const std::vector<Frame> frames;

    /**
     * @brief Index of the frame currently being used.
     * 
     */
    TileId currentFrame;

    /**
     * @brief Length of time until the next frame, in seconds.
     * 
     */
    double timer;

    /**
     * @brief Index of the tile to animate.
     * 
     */
    const TileId tile;
};
