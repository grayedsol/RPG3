/**
 * @file Tile.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Defines basic Tile::Tile and Tile::Animation types.
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "ECS.hpp"
#include <vector>

namespace Tile {
	using TileId = uint16_t;
	using CollisionId = uint16_t;
	using TilesetId = uint8_t;
	using entity = ECS::entity;
	/**
	 * @brief A lightweight game entity.
	 * 
	 */
	struct Tile {
		/**
		 * @brief Identifier to lookup in a tileset.
		 * 
		 */
		TileId id;

		/**
		 * @brief Identifier for collision data.
		 * 
		 */
		CollisionId collision = 0;
	};

	/**
	 * @brief Specifications for animating a tile.
	 * 
	 */
	struct Animation {
		/**
		 * @brief Single image in the animation.
		 * 
		 */
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
};
