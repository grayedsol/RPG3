/**
 * @file CommandMap.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief CommandMap
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "GameCommands.hpp"
#include "VirtualButton.hpp"
#include "GRY_Log.hpp"

/**
 * @brief Map of game commands to virtual buttons, and vice versa.
 * 
 */
struct CommandMap {
    /**
     * @brief Array that maps VirtualButtons to GCmds.
     * 
     */
	GCmd commands[VirtualButton::VIRTUAL_BUTTON_SIZE];

    /**
     * @brief Array that maps GCmds to VirtualButtons.
     * 
     */
    VirtualButton buttons[GCmd::GCMD_SIZE];

    /**
     * @brief Reset the command set.
     * 
     * @details
     * Effectively turns of all input coming through this command set until
     * the commands are mapped again.
     */
    void resetCmds() {
        for (int i = 0; i < VirtualButton::VIRTUAL_BUTTON_SIZE; i++) {
			commands[i] = GCmd::GCMD_NONE;
		}
        for (int i = 0; i < GCmd::GCMD_SIZE; i++) {
            buttons[i] = VirtualButton::GAME_NONE;
        }
    }

    /**
     * @brief Constructor.
     * 
     * @details
     * All GCmds are set to GCmd::GCMD_NONE, and all
     * VirtualButtons are set to VirtualButton::GAME_NONE.
     * 
     */
	CommandMap() { resetCmds(); }

	/**
     * @brief Map a game command to a virtual button, and vice-versa.
     * 
     * @details
     * Logs an error and does not update the mapping if GCmd::GCMD_NONE or VirtualButton::GAME_NONE
     * are used in either parameter.
     * 
     * @param command GCmd to map to "button".
     * @param button VirtualButton to map to "command".
     */
	void mapCmd(GCmd command, VirtualButton button) {
        if (command == GCmd::GCMD_NONE || button == VirtualButton::GAME_NONE) {
            GRY_Log("[CommandMap] Cannot map GCmd::GCMD_NONE or VirtualButton::GAME_NONE.\n");
            return;
        }
        commands[button] = command;
        buttons[command] = button;
    }
};
