#pragma once
#include "GRY_VecTD.hpp"

struct Position2Tag {};
using Position2 = GRY_VecTD<float, 2, Position2Tag>;

struct Velocity2Tag {};
using Velocity2 = GRY_VecTD<float, 2, Velocity2Tag>;

struct Hitbox {
	float x;
	float y;
	float w;
	float h;
};

inline bool operator==(Hitbox lhs, Hitbox rhs) {
	return lhs.x == rhs.x &&
	lhs.y == rhs.y &&
	lhs.w == rhs.w &&
	lhs.h == rhs.h;
}

inline bool operator!=(Hitbox lhs, Hitbox rhs) {
	return lhs.x != rhs.x ||
	lhs.y != rhs.y ||
	lhs.w != rhs.w ||
	lhs.h != rhs.h;
}
