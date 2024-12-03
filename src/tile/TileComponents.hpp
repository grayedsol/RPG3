#pragma once
#include "SDL3/SDL.h"
#include <stdint.h>
#include "Tile.hpp"

struct Actor {
	enum Direction {
		NONE = 0,
		
		Down = 1,
		Up = 2,
		Left = 3,
		LeftDown = 4,
		LeftUp = 5,
		Right = 6,
		RightDown = 7,
		RightUp = 8,
		
		SIZE = 9
	};

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
};

struct ActorSprite {
	using TileId = Tile::TileId;
	using TilesetId = Tile::TilesetId;

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

struct Player {};
