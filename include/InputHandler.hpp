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
 * @brief Mouse button representation.
 * 
 * @details
 * See SDL_mouse.h.
 */
enum GRY_Mousecode {
    GRY_MOUSE_UNKNOWN = 0,
    GRY_MOUSE_LEFT = 1,
    GRY_MOUSE_MIDDLE = 2,
    GRY_MOUSE_RIGHT = 3,
    GRY_MOUSE_X1 = 4,
    GRY_MOUSE_X2 = 5,
    GRY_NUM_MOUSECODES = 6
};

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
    VirtualButton keyButtons[SDL_NUM_SCANCODES];

    /**
     * @brief Array that maps GRY_Mousecodes to VirtualButtons.
     * 
     */
    VirtualButton mouseButtons[GRY_NUM_MOUSECODES];

    const Uint8* VButtonState[VirtualButton::VIRTUAL_BUTTON_SIZE][2];

    Uint8 mouseState[GRY_NUM_MOUSECODES];

    /**
     * @brief Points to the keyboard state obtained from `SDL_GetKeyboardState()`.
     * 
     */
    const Uint8* keyboardState = NULL;

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
     * @brief Map a virtual button to a code, and vice-versa.
     * 
     * @param code Code to map `button` to.
     * @param button VirtualButton to map `code` to.
     */
    void mapInput(unsigned int code, VirtualButton button, bool mouse, bool secondary = false);

    /**
     * @brief Reset the mapping of VirtualButtons and SDL_Scancodes.
     * 
     */
    void resetControls();

    /**
     * @brief Process a mouse button down SDL_Event.
     * 
     * @param e 
     */
    void processMouseInput(const SDL_Event& e);

    /**
     * @brief Process a key up or key down SDL_Event.
     * 
     * @param e Reference to the SDL_Event to process.
     */
    void processKeyInput(const SDL_Event& e);
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
     * @sa isPressing
     */
    const VirtualButton getInput() const { 
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
     * @sa getInput
     * @sa isPressing
     */
    const VirtualButton getSingleInput() const { return singleInput; }

    /**
	 * @brief Determine if `b` is being pressed..
	 * 
	 * @param b VirtualButton to check.
	 * @return `true` if `b` is being pressed.
	 * @return `false` otherwise.
     * 
     * @sa getInput
     * @sa getSingleInput
	 */
    const bool isPressing(VirtualButton b) const;
};
