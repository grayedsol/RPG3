/**
 * @file TileComponents.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Component structures for a tile based game.
 * @copyright Copyright (c) 2025
 */
#pragma once
#include <stdint.h>
#include "Tile.hpp"

namespace Tile {
	enum Direction : uint8_t {
		DirectionNone = 0,
		
		Down = 1,
		Up = 2,
		Left = 3,
		LeftDown = 4,
		LeftUp = 5,
		Right = 6,
		RightDown = 7,
		RightUp = 8,
		
		DirectionSize = 9
	};

	struct Actor {
		/**
		 * @brief Movement speed.
		 * 
		 */
		float speed = 1.f;

		/**
		 * @brief Direction the actor is facing.
		 *
		 */
		Direction direction = Direction::Down;

		/**
		 * @brief The map layer the actor is on.
		 * 
		 */
		uint8_t layer = 0;

		/**
		 * @brief Whether the actor is moving or not.
		 * 
		 */
		bool moving = false;

		/**
		 * @brief Whether the actor is sprinting or not.
		 * 
		 */
		bool sprinting = false;
	};

	struct ActorSprite {
		/**
		 * @brief X coordinate rendering offset, in game pixels
		 * 
		 */
		float offsetX;

		/**
		 * @brief Y coordinate rendering offset, in game pixels
		 * 
		 */
		float offsetY;

		/**
		 * @brief Index of the tile to render
		 * 
		 */
		TileId index;

		/**
		 * @brief Index of the tileset to use
		 *
		 */
		TilesetId tileset;
	};

	struct ActorSpriteAnims {		
		std::vector<TileId> walk[Direction::DirectionSize];
		std::vector<TileId> sprint[Direction::DirectionSize];

		double duration;
		unsigned index = 0;
		double timer = 0;
	};

	struct Player {
		entity speakingTo = ECS::NONE;
	};

	struct NPC {};
};
