/**
 * @file GRY_Mousecode.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief GRY_Mousecode
 * @copyright Copyright (c) 2024
 */
#pragma once

/**
 * @brief Mouse button representation.
 * 
 * @details
 * Meant to be a mouse equivalent to SDL_Scancode.
 * See SDL_scancode.h and SDL_mouse.h.
 */
enum GRY_Mousecode {
	GRY_MOUSECODE_UNKNOWN = 0,
	GRY_MOUSECODE_LEFT = 1,
	GRY_MOUSECODE_MIDDLE = 2,
	GRY_MOUSECODE_RIGHT = 3,
	GRY_MOUSECODE_X1 = 4,
	GRY_MOUSECODE_X2 = 5,
	GRY_MOUSECODE_COUNT = 6
};
