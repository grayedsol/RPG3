#pragma once
#include "SDL3/SDL.h"
#include <stdint.h>

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
	 * @brief Direction the actor is facing.
	 *
	 */
	Direction direction;
	
	/**
	 * @brief Index of the tileset to use
	 *
	 */
	uint8_t tileset = 0;
};

struct ActorTexture {
	uint16_t data[Actor::Direction::SIZE];
};
