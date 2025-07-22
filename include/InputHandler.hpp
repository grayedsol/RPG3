/**
 * @file InputHandler.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief InputHandler
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "SDL3/SDL_scancode.h"
#include "GRY_Mousecode.hpp"
#include "CommandMap.hpp"
#include <vector>

/**
 * @brief Handles conversion and reading of input from SDL to VirtualButtons.
 * 
 */
class InputHandler {
private:
    /**
     * @brief Array that maps SDL_Scancodes to VirtualButtons.
     * 
     */
    VirtualButton keyButtons[SDL_SCANCODE_COUNT];

    /**
     * @brief Array that maps GRY_Mousecodes to VirtualButtons.
     * 
     */
    VirtualButton mouseButtons[GRY_MOUSECODE_COUNT];

    /**
     * @brief Array that can be used to check the pressing state of a VirtualButton.
     * 
     * @details
     * The array can be indexed using a VirtualButton and 0 or 1 as the index to the
     * primary or secondary binding, respectively.
     * The bindings point to the state of a physical mouse or keyboard input.
     * 
     * @sa isPressingVButton
     */
    const bool* buttonState[VirtualButton::VIRTUAL_BUTTON_SIZE][2];

    /**
     * @brief Array representing the state of each mouse button.
     * 
     */
    bool mouseState[GRY_MOUSECODE_COUNT];

    /**
     * @brief Points to the keyboard state obtained from `SDL_GetKeyboardState()`.
     * 
     */
    const bool* keyboardState = NULL;

    /**
     * @brief A list of inputs. Only `inputs.back()` is guaranteed to be currently pressed.
     * 
     * @details
     * Any inputs other than `inputs.back()` may or may not be currently pressed, but
     * are stored anyways to keep track of inputs to "fall back" to when `inputs.back()`
     * is released.
     */
	std::vector<VirtualButton> inputs;

    /**
     * @brief Stores the latest input from the current frame only.
     * 
     * @details
     * It is GAME_NONE if there have been no inputs from the current frame.
     * 
     */
	VirtualButton singleInput = GAME_NONE;

	/**
	 * @brief The command map control scheme of the game.
	 * 
	 */
	CommandMap controlScheme;

    /**
     * @brief Map a virtual button to a physical input, and vice-versa.
     * 
     * @param code Code of the physical input to map `button` to.
     * @param button VirtualButton to map `code` to.
     */
    void mapInput(unsigned int code, VirtualButton button, bool mouse, bool secondary = false);

    /**
     * @brief Reset the mapping of VirtualButtons and SDL_Scancodes.
     * 
     */
    void resetControls();

    /**
     * @brief Loads the controls scheme.
     * 
     */
    void loadControls();
public:
    /**
     * @brief Constructor.
     * 
     */
    InputHandler();

    /**
     * @brief Process input.
     * 
     * @param gameRunning Reference to the game's running status, which may be switched.
     */
    void process(bool& gameRunning);

	/**
	 * @brief Set the control scheme.
	 * 
	 * @param map Map to set the control scheme to
	 */
	void setControlScheme(CommandMap map) { controlScheme = map; }

    /**
     * @brief Get the latest active VirtualButton input.
     * 
     * @details
     * Returns VirtualButton::GAME_NONE if there are no active inputs.
     * 
     * @return A VirtualButton input.
     * 
     * @sa getSingleInputVButton
     * @sa isPressingVButton
     */
    const VirtualButton getInputVButton() const { 
        return inputs.size() ? inputs.back() : VirtualButton::GAME_NONE;
    }

    /**
     * @brief Get the latest active VirtualButton input from the current frame only.
     * 
     * @details
     * Returns VirtualButton::GAME_NONE if there are no active inputs from this frame.
     * 
     * @return A VirtualButton input.
     * 
     * @sa getInputVButton
     * @sa isPressingVButton
     */
    const VirtualButton getSingleInputVButton() const { return singleInput; }

    /**
	 * @brief Determine if `b` is being pressed.
	 * 
	 * @param b VirtualButton to check.
	 * @return `true` if `b` is being pressed.
	 * @return `false` otherwise.
     * 
     * @sa getInputVButton
     * @sa getSingleInputVButton
	 */
    const bool isPressingVButton(VirtualButton b) const {
	    return b &&
		    ((buttonState[b][0] && *buttonState[b][0]) ||
		    (buttonState[b][1] && *buttonState[b][1]));
    }

    /**
     * @brief Get the latest active GCmd input.
     * 
     * @details
     * Returns GCmd::GCMD_NONE if there are no active inputs.
     * 
     * @return A GCmd input.
     * 
     * @sa getSingleInput
     * @sa isPressing
     */
	const GCmd getInput() const { return controlScheme.commands[getInputVButton()]; }

    /**
     * @brief Get the latest active GCmd input from the current frame only.
     * 
     * @details
     * Returns GCmd::GCMD_NONE if there are no active inputs from this frame.
     * 
     * @return A GCmd input.
     * 
     * @sa getInput
     * @sa isPressing
     */
	const GCmd getSingleInput() const { return controlScheme.commands[singleInput]; }

    /**
	 * @brief Determine if the VirtualButton mapped to `cmd` is being pressed.
	 * 
	 * @param cmd GCmd to check.
	 * @return `true` if `cmd` is being pressed.
	 * @return `false` otherwise.
     * 
     * @sa getInput
     * @sa getSingleInput
	 */
	const bool isPressing(GCmd cmd) const {
		return isPressingVButton(controlScheme.buttons[cmd]);
	}
};
