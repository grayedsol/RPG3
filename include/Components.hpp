#pragma once
#include "GRY_VecTD.hpp"

struct Position2Tag {};
using Position2 = GRY_VecTD<float, 2, Position2Tag>;

struct Velocity2Tag {};
using Velocity2 = GRY_VecTD<float, 2, Velocity2Tag>;

struct Hitbox {
	Position2 center;
	float halfWidth;
	float halfHeight;
};
