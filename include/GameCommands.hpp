/**
 * @file GameCommands.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Enum type that correlates to an in-game action.
 * @copyright Copyright (c) 2024
 */
#pragma once

/**
 * @brief Directly correlates to an in-game action.
 * 
 */
enum GCmd {
	GCMD_NONE,
	
	GameMenu,
	GameQuit,
	GameUp,
	GameDown,
	GameLeft,
	GameRight,

	MapInteract,
	MapSprint,
	MapMenu,
	MapUp,
	MapDown,
	MapLeft,
	MapRight,
	
	MessageOk,
	MessageUp,
	MessageDown,

	MenuOk,
	MenuBack,
	MenuUp,
	MenuDown,
	MenuLeft,
	MenuRight,

	BattleUp,
	BattleDown,
	BattleLeft,
	BattleRight,

	GCMD_SIZE
};