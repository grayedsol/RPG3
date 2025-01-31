/**
 * @file SDL_FRectOps.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Operator overloads for SDL_FRect
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "SDL3/SDL_rect.h"

inline void operator*=(SDL_FRect& rect, float scalar) {
	rect.x *= scalar;
	rect.y *= scalar;
	rect.w *= scalar;
	rect.h *= scalar;
}

inline void operator*=(SDL_Rect& rect, int scalar) {
	rect.x *= scalar;
	rect.y *= scalar;
	rect.w *= scalar;
	rect.h *= scalar;
}