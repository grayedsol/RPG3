/**
 * @file VirtualButton.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Enum type that correlates to a theoretical button.
 * @copyright Copyright (c) 2024
 */
#pragma once

/**
 * @brief Correlates to a theoretical button.
 * 
 */
enum VirtualButton {
	GAME_NONE = 0,

	GAME_A = 1,
	GAME_B = 2,
	GAME_X = 3,
	GAME_Y = 4,
	GAME_UP = 5,
	GAME_DOWN = 6,
	GAME_LEFT = 7,
	GAME_RIGHT = 8,
	GAME_LT = 9,
	GAME_RT = 10,
	GAME_LB = 11,
	GAME_RB = 12,
	GAME_START = 13,
	GAME_SELECT = 14,
	
	VIRTUAL_BUTTON_SIZE = 15
};

static const char* VirtualButtonStrings[] = {
	"GAME_NONE",

	"GAME_A",
	"GAME_B",
	"GAME_X",
	"GAME_Y",
	"GAME_UP",
	"GAME_DOWN",
	"GAME_LEFT",
	"GAME_RIGHT",
	"GAME_LT",
	"GAME_RT",
	"GAME_LB",
	"GAME_RB",
	"GAME_START",
	"GAME_SELECT",

	"VIRTUAL_BUTTON_SIZE"
};
