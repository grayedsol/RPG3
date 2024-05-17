/**
 * @file InputHandler.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief InputHandler
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "SDL3/SDL.h"
#include "VirtualButton.hpp"
#include "GRY_Log.hpp"
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
    VirtualButton buttons[SDL_NUM_SCANCODES];

    /**
     * @brief Array that maps VirtualButtons to SDL_Scancodes.
     * 
     */
	SDL_Scancode scancodes[VirtualButton::VIRTUAL_BUTTON_SIZE];

    /**
     * @brief Points to the keyboard state obtained from `SDL_GetKeyboardState()`.
     * 
     */
    const Uint8* keyboardState = NULL;

    /**
     * @brief A list of inputs. Only `keyInputs.back()` is guaranteed to be currently pressed.
     * 
     * @details
     * Any inputs other than `keyInputs.back()` may or may not be currently pressed, but
     * are stored anyways to keep track of inputs to "fall back" to when `keyInputs.back()`
     * is released.
     */
	std::vector<SDL_Scancode> keyInputs;

    /**
     * @brief Stores the latest input from the current frame only.
     * 
     * @details
     * It is SDL_SCANCODE_UNKNOWN if there have been no inputs from the current frame.
     * 
     */
	SDL_Scancode singleKeyInput = SDL_SCANCODE_UNKNOWN;

    /**
     * @brief Map a virtual button to a scancode, and vice-versa.
     * 
     * @param scancode Scancode to map `button` to.
     * @param button VirtualButton to map `scancode` to.
     */
    void mapButton(SDL_Scancode scancode, VirtualButton button);

    /**
     * @brief Reset the mapping of VirtualButtons and SDL_Scancodes.
     * 
     */
    void resetControls();

    /**
     * @brief Process a key up or key down SDL_Event.
     * 
     * @param e Reference to the SDL_Event to process.
     */
    void processInput(SDL_Event& e);
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
     * @brief Get the latest active VirtualButton input.
     * 
     * @details
     * Returns VirtualButton::GAME_NONE if there are no active inputs.
     * 
     * @return A VirtualButton input.
     * 
     * @sa getSingleInput
     * @sa getScancodeInput
     * @sa getSingleScancodeInput
     * @sa isPressing
     */
    const VirtualButton getInput() const { 
        return keyInputs.size() ? buttons[keyInputs.back()] : VirtualButton::GAME_NONE;
    }

    /**
     * @brief Get the latest active VirtualButton input from the current frame only.
     * 
     * @details
     * Returns VirtualButton::GAME_NONE if there are no active inputs from this frame.
     * 
     * @return A VirtualButton input.
     * 
     * @sa getInput
     * @sa getScancodeInput
     * @sa getSingleScancodeInput
     * @sa isPressing
     */
    const VirtualButton getSingleInput() const { return buttons[singleKeyInput]; }

    /**
     * @brief Get the latest active SDL_Scancode input.
     * 
     * @details
     * Returns SDL_SCANCODE_UNKNOWN if there are no active inputs.
     * 
     * @return An SDL_Scancode input.
     * 
     * @sa getInput
     * @sa getSingleInput
     * @sa getSingleScancodeInput
     * @sa isPressing
     */
    const SDL_Scancode getScancodeInput() const {
        return keyInputs.size() ? keyInputs.back() : SDL_SCANCODE_UNKNOWN;
    }

    /**
     * @brief Get the latest active SDL_Scancode input from the current frame only.
     * 
     * @details
     * Returns SDL_SCANCODE_UNKNOWN if there are no active inputs from this frame.
     * 
     * @return An SDL_Scancode input.
     * 
     * @sa getInput
     * @sa getSingleInput
     * @sa getScancodeInput
     * @sa isPressing
     */
    const SDL_Scancode getSingleScancodeInput() const { return singleKeyInput; }

    /**
	 * @brief Determine if `b` is being pressed by checking its associated SDL_Scancode.
	 * 
	 * @param b VirtualButton to check.
	 * @return `true` if `b` is being pressed.
	 * @return `false` otherwise.
     * 
     * @sa getInput
     * @sa getSingleInput
     * @sa getScancodeInput
     * @sa getSingleScancodeInput
	 */
    const bool isPressing(VirtualButton b) const { return (bool)keyboardState[scancodes[b]]; }
};
