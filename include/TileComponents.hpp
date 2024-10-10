#pragma once
#include "SDL3/SDL.h"
#include <stdint.h>
#include "Tile.hpp"

struct Actor {
	enum Direction {
		Down = 0,
		Up = 1,
		Left = 2,
		Right = 3,
		DownLeft = 4,
		DownRight = 5,
		UpLeft = 6,
		UpRight = 7,

		SIZE = 8
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

struct ActorTextureList {
	using TileId = Tile::TileId;
	TileId data[Actor::Direction::SIZE];
};

struct Player {};
